#include <glad.h>
#include <GLFW/glfw3.h>

#include <typeinfo>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "../support/error.hpp"
#include "../support/program.hpp"
#include "../support/checkpoint.hpp"
#include "../support/debug_output.hpp"

#include "../vmlib/vec3.hpp"
#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"
#include "../vmlib/mat33.hpp"

#include "defaults.hpp"
#include "loadobj.hpp"
#include "mesh.hpp"
#include "spaceship.hpp"
#include "texture.hpp"

namespace
{
  constexpr char const* kWindowTitle = "COMP3811 - CW2";

  constexpr float kPi_ = 3.1415926f;

  constexpr float kMouseSensitivity_ = 0.01f; // radians per pixel

  struct State_
  {
    ShaderProgram* prog;

    struct Camera_
    {
      bool cameraActive;
      bool
      actionForward,
      actionBackward,
      actionLeft,
      actionRight,
      actionUp,
      actionDown;

      float speedMul;

      Vec4f pos;

      float yaw, pitch;

      float lastX, lastY;
    } camera;

    struct SpaceshipControls_
    {
      bool moving, reset;
      Vec3f pos;
    }spaceship_controls;

    double lastPrintTime = 0.0; // Last print time for cam pos
  };

  void glfw_callback_error_( int, char const* );

  void glfw_callback_key_(GLFWwindow*, int, int, int, int);
  void glfw_callback_motion_(GLFWwindow*, double, double);

  struct GLFWCleanupHelper
  {
    ~GLFWCleanupHelper();
  };
  struct GLFWWindowDeleter
  {
    ~GLFWWindowDeleter();
    GLFWwindow* window;
  };
}

int main() try
{
  // Initialize GLFW
  if( GLFW_TRUE != glfwInit() )
  {
    char const* msg = nullptr;
    int ecode = glfwGetError( &msg );
    throw Error( "glfwInit() failed with '%s' (%d)", msg, ecode );
  }

  // Ensure that we call glfwTerminate() at the end of the program.
  GLFWCleanupHelper cleanupHelper;

  // Configure GLFW and create window
  glfwSetErrorCallback( &glfw_callback_error_ );

  glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
  glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

  //glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

  glfwWindowHint( GLFW_DEPTH_BITS, 24 );

#	if !defined(NDEBUG)
  // When building in debug mode, request an OpenGL debug context. This
  // enables additional debugging features. However, this can carry extra
  // overheads. We therefore do not do this for release builds.
  glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#	endif // ~ !NDEBUG

  GLFWwindow* window = glfwCreateWindow(
      1280,
      720,
      kWindowTitle,
      nullptr, nullptr
      );

  if( !window )
  {
    char const* msg = nullptr;
    int ecode = glfwGetError( &msg );
    throw Error( "glfwCreateWindow() failed with '%s' (%d)", msg, ecode );
  }

  GLFWWindowDeleter windowDeleter{ window };

  // Set up event handling
  // TODO: Additional event handling setup
  State_ state{
  };

  glfwSetWindowUserPointer( window, &state );

  glfwSetKeyCallback( window, &glfw_callback_key_ );
  glfwSetCursorPosCallback( window, &glfw_callback_motion_ );

  // Set up drawing stuff
  glfwMakeContextCurrent( window );
  glfwSwapInterval( 1 ); // V-Sync is on.

  // Initialize GLAD
  // This will load the OpenGL API. We mustn't make any OpenGL calls before this!
  if( !gladLoadGLLoader( (GLADloadproc)&glfwGetProcAddress ) )
    throw Error( "gladLoaDGLLoader() failed - cannot load GL API!" );

  std::printf( "RENDERER %s\n", glGetString( GL_RENDERER ) );
  std::printf( "VENDOR %s\n", glGetString( GL_VENDOR ) );
  std::printf( "VERSION %s\n", glGetString( GL_VERSION ) );
  std::printf( "SHADING_LANGUAGE_VERSION %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

  // Ddebug output
#	if !defined(NDEBUG)
  setup_gl_debug_output();
#	endif // ~ !NDEBUG

  // Setup paths
#if defined(WIN32)
  const char* defaultVertexShaderPath = "../assets/default.vert";
  const char* defaultFragmentShaderPath = "../assets/default.frag";
  const char* terrainObjPath = "../assets/parlahti.obj";
  const char* textureObjPath = "../assets/L4343A-4k.jpeg";
  const char* launchpadObjPath = "../assets/landingpad.obj";
#else
  const char* defaultVertexShaderPath = "assets/default.vert";
  const char* defaultFragmentShaderPath = "assets/default.frag";
  const char* terrainObjPath = "assets/parlahti.obj";
  const char* textureObjPath = "assets/L4343A-4k.jpeg";
  const char* launchpadObjPath = "assets/landingpad.obj";
#endif

  // Global GL state
  OGL_CHECKPOINT_ALWAYS();

  // TODO: global GL setup goes here
  glEnable(GL_FRAMEBUFFER_SRGB);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.2f, .2f, 0.0f);

  OGL_CHECKPOINT_ALWAYS();

  // Get actual framebuffer size.
  // This can be different from the window size, as standard window
  // decorations (title bar, borders, ...) may be included in the window size
  // but not be part of the drawable surface area.
  int iwidth, iheight;
  glfwGetFramebufferSize( window, &iwidth, &iheight );

  glViewport( 0, 0, iwidth, iheight );

  // Loads the shader program
  ShaderProgram prog( {
      { GL_VERTEX_SHADER, defaultVertexShaderPath },
      { GL_FRAGMENT_SHADER, defaultFragmentShaderPath }
      } );

  state.prog = &prog;
  state.camera.pos = {25.f, 5.f, -10.f, 1.f};
  state.camera.pitch = 0.f;
  state.camera.yaw   = kPi_ / -2.f;
  state.camera.speedMul = 0.2f;

  state.spaceship_controls.moving = false;
  state.spaceship_controls.reset = false;
  state.spaceship_controls.pos = {25.f, -0.77f, -6.f};

  // Animation state
  auto last = Clock::now();

  float angle = 0.f;

  // Other initialization & loading
  OGL_CHECKPOINT_ALWAYS();

  // Create vertex buffers and VAO
  auto terrain_mesh = load_wavefront_obj(terrainObjPath);
  GLuint terrain_vao = create_vao(terrain_mesh);
  std::size_t terrainVertexCount = terrain_mesh.positions.size();

  // Load terrain texture
  auto textureObjectId = load_texture_2d(textureObjPath);

  auto spaceship_mesh = make_spaceship();
  GLuint spaceship_vao = create_vao(spaceship_mesh);
  std::size_t spaceshipVertexCount = spaceship_mesh.positions.size();

  // Landingpad
  auto landingpad_mesh = load_wavefront_obj(launchpadObjPath);
  GLuint landingpad_vao = create_vao(landingpad_mesh);
  std::size_t landingpadVertexCount = landingpad_mesh.positions.size();

  Mat44f landingpadTransform1 = make_translation({ -43.0f, -0.97f, 8.f });
  Mat44f landingpadTransform2 = make_translation({ 25.0f, -0.97f, -6.f });

  OGL_CHECKPOINT_ALWAYS();

  float spaceship_clock = 0.f;

  // Main loop
  while( !glfwWindowShouldClose( window ) )
  {
    // Let GLFW process events
    glfwPollEvents();

    // Check if window was resized.
    float fbwidth, fbheight;
    {
      int nwidth, nheight;
      glfwGetFramebufferSize( window, &nwidth, &nheight );

      fbwidth = float(nwidth);
      fbheight = float(nheight);

      if( 0 == nwidth || 0 == nheight )
      {
        // Window minimized? Pause until it is unminimized.
        // This is a bit of a hack.
        do
        {
          glfwWaitEvents();
          glfwGetFramebufferSize( window, &nwidth, &nheight );
        } while( 0 == nwidth || 0 == nheight );
      }

      glViewport( 0, 0, nwidth, nheight );
    }

    // Update state
    auto const now = Clock::now();
    float dt = std::chrono::duration_cast<Secondsf>(now-last).count();
    last = now;

    // Update camera state
    if(state.camera.actionForward)
    { 
      float yaw = state.camera.yaw;
      yaw += kPi_ / 2.f;
      if (yaw > 2.f * kPi_)
        yaw = -2.f * kPi_ + (2.f*kPi_ - yaw);

      state.camera.pos.x -= cos(yaw) * state.camera.speedMul;
      state.camera.pos.z -= sin(yaw) * state.camera.speedMul;
    }
    else if(state.camera.actionBackward)
    {
      float yaw = state.camera.yaw;
      yaw += kPi_ / 2.f;
      if (yaw > 2.f * kPi_)
        yaw = -2.f * kPi_ + (2.f*kPi_ - yaw);

      state.camera.pos.x += cos(yaw) * state.camera.speedMul;
      state.camera.pos.z += sin(yaw) * state.camera.speedMul;
    }
    else if(state.camera.actionLeft)
    {
      float yaw = state.camera.yaw;
      state.camera.pos.x -= cos(yaw) * state.camera.speedMul;
      state.camera.pos.z -= sin(yaw) * state.camera.speedMul;
    }
    else if(state.camera.actionRight)
    {
      float yaw = state.camera.yaw;
      state.camera.pos.x += cos(yaw) * state.camera.speedMul;
      state.camera.pos.z += sin(yaw) * state.camera.speedMul;
    }
    else if(state.camera.actionUp)
    {
      state.camera.pos.y += 0.2f * state.camera.speedMul;
    }
    else if(state.camera.actionDown)
    {
      state.camera.pos.y -= 0.2f * state.camera.speedMul;
    }

    // Update: move spaceship
    if (state.spaceship_controls.moving == true)
    {
      spaceship_clock += dt;
      spaceship_mesh = move_spaceship(spaceship_mesh,
                                      spaceship_clock,
                                      &state.spaceship_controls.pos);
    }
    if (state.spaceship_controls.reset == true)
    {
      spaceship_clock = 0.f;
      spaceship_mesh = make_spaceship();

      state.spaceship_controls.reset = false;
    }
    GLuint spaceship_vao = create_vao(spaceship_mesh);

    // Update: compute matrices
    Mat44f model2world = make_rotation_y(angle);

    Mat44f Rx = make_rotation_x(state.camera.pitch);
    Mat44f Ry = make_rotation_y(state.camera.yaw);
    Mat44f T = make_translation({-state.camera.pos.x,
                                 -state.camera.pos.y,
                                 -state.camera.pos.z});

    // First person camera
    Mat44f world2camera = Rx * Ry * T;

    // Arc-ball camera
    // Mat44f world2camera = T * Rx * Ry;

    Mat44f projection = make_perspective_projection(
        60.f * kPi_ / 180.f,
        fbwidth/float(fbheight),
        0.1f, 200.f
        );

    Mat44f projCameraWorld = projection * world2camera * model2world;

    Mat33f normalMatrix = mat44_to_mat33(transpose(invert(model2world)));

    // Draw scene
    OGL_CHECKPOINT_DEBUG();

    //Draw frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(prog.programId());

    glUniformMatrix4fv(
        0,
        1, GL_TRUE, projCameraWorld.v
        );

    glUniformMatrix3fv(
      1,
      1, GL_TRUE, normalMatrix.v
    );
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureObjectId);

    // World Light
    Vec3f lightDir = normalize(Vec3f{0.f, 1.f, -1.f});
    // LightDir
    glUniform3fv(2, 1, &lightDir.x);
    // LightDiffuse
    glUniform3f(3, 0.9f, 0.9f, 0.9f);
    // SceneAmbient
    glUniform3f(4, 0.05f, 0.05f, 0.05f);
  
    // ------------------------------- 2D GUI --------------------------------

    

    // ------------------------------- TERRAIN -------------------------------

    // Tell shader that we are using texture
    glUniform1i(glGetUniformLocation(prog.programId(), "uUseTexture"), GL_TRUE);
    // Bind texture to terrain
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureObjectId);

    // Light for terrain
    Mat44f terrainModelMatrix = kIdentity44f;
    glUniformMatrix4fv(13, 1, GL_TRUE, terrainModelMatrix.v);

    glBindVertexArray(terrain_vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, terrainVertexCount);

    // We don't need terrain's texture after
    glBindTexture(GL_TEXTURE_2D, 0);
    // We are not using texture from here
    glUniform1i(glGetUniformLocation(prog.programId(), "uUseTexture"), GL_FALSE);
   

    // ------------------------------- SPACE SHIP -------------------------------

    // Pass space ship model position to shader
    
    // Point lights for point light 1
    Vec3f pointLightPosition1 = { 1.0f, 2.0f, 3.0f };  
    Vec3f pointLightDiffuseColor1 = { 0.f, 1.f, 1.0f }; 
    Vec3f pointLightSpecularColor1 = { 1.0f, 1.0f, 0.f }; 
    glUniform3fv(9, 1, &pointLightPosition1.x);
    glUniform3fv(11, 1, &pointLightDiffuseColor1.x);
    glUniform3fv(12, 1, &pointLightSpecularColor1.x);

    // Point lights for point light 2
    Vec3f pointLightPosition2 = { 20.0f, 2.0f, 20.0f };  
    Vec3f pointLightDiffuseColor2 = { 0.f, 1.f, 1.0f }; 
    Vec3f pointLightSpecularColor2 = { 1.0f, 1.0f, 0.f }; 
    glUniform3fv(9, 1, &pointLightPosition2.x);
    glUniform3fv(11, 1, &pointLightDiffuseColor2.x);
    glUniform3fv(12, 1, &pointLightSpecularColor2.x);

    // Point lights for point light 3
    Vec3f pointLightPosition3 = {50.0f, 2.0f, -30.0f };  
    Vec3f pointLightDiffuseColor3 = { 0.f, 1.f, 1.0f }; 
    Vec3f pointLightSpecularColor3 = { 1.0f, 1.0f, 0.f }; 
    glUniform3fv(9, 1, &pointLightPosition3.x);
    glUniform3fv(11, 1, &pointLightDiffuseColor3.x);
    glUniform3fv(12, 1, &pointLightSpecularColor3.x);

    glBindVertexArray(spaceship_vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, spaceshipVertexCount);
  

    // ------------------------------- LANDING PAD -------------------------------
    
    // Light for landing pad
    // Mat44f landingpadModelMatrix = kIdentity44f;
    //glUniformMatrix4fv(13, 1, GL_TRUE, landingpadModelMatrix.v);

    //Vec3f landingpadSpecularColor = { 0.f, 0.f, 1.f };
    //glUniform3fv(6, 1, &landingpadSpecularColor.x);
    float landingpadShininess = 32.0f;
    glUniform1f(7, landingpadShininess);

    Mat44f model1 = landingpadTransform1; 
    Mat44f projCameraWorld1 = projection * world2camera * model1;
    glUniformMatrix4fv(0, 1, GL_TRUE, projCameraWorld1.v);
    glBindVertexArray(landingpad_vao);
    glDrawArrays(GL_TRIANGLES, 0, landingpadVertexCount);

    Mat44f model2 = landingpadTransform2;
    Mat44f projCameraWorld2 = projection * world2camera * model2;
    glUniformMatrix4fv(0, 1, GL_TRUE, projCameraWorld2.v);
    glBindVertexArray(landingpad_vao);
    glDrawArrays(GL_TRIANGLES, 0, landingpadVertexCount);



    double currentTime = glfwGetTime();

    // Print cam position help to place landing pad
    if (currentTime - state.lastPrintTime >= 1.0) {
        std::printf("Camera Position: X = %.2f, Y = %.2f, Z = %.2f\n",
            state.camera.pos.x, state.camera.pos.y, state.camera.pos.z);
        std::printf("Spaceship Position: X = %.2f, Y = %.2f, Z = %.2f\n",
            state.spaceship_controls.pos.x,
            state.spaceship_controls.pos.y,
            state.spaceship_controls.pos.z);
        state.lastPrintTime = currentTime;
    }
    

    OGL_CHECKPOINT_DEBUG();

    // Display results
    glfwSwapBuffers( window );
  }

  // Cleanup.
  //TODO: additional cleanup

  return 0;
}
catch( std::exception const& eErr )
{
  std::fprintf( stderr, "Top-level Exception (%s):\n", typeid(eErr).name() );
  std::fprintf( stderr, "%s\n", eErr.what() );
  std::fprintf( stderr, "Bye.\n" );
  return 1;
}


namespace
{
  void glfw_callback_error_( int aErrNum, char const* aErrDesc )
  {
    std::fprintf( stderr, "GLFW error: %s (%d)\n", aErrDesc, aErrNum );
  }

  void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int )
  {
    if( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction )
    {
      glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
      return;
    }

    if( auto* state = static_cast<State_*>(glfwGetWindowUserPointer( aWindow )) )
    {
      // Space toggles camera
      if( GLFW_KEY_SPACE == aKey && GLFW_PRESS == aAction )
      {
        state->camera.cameraActive = !state->camera.cameraActive;

        if( state->camera.cameraActive )
          glfwSetInputMode( aWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
        else
          glfwSetInputMode( aWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
      }

      // Camera controls if camera is active
      if( state->camera.cameraActive )
      {
        if( GLFW_KEY_W == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera.actionForward = true;
          else if( GLFW_RELEASE == aAction )
            state->camera.actionForward = false;
        }
        else if( GLFW_KEY_S == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera.actionBackward = true;
          else if( GLFW_RELEASE == aAction )
            state->camera.actionBackward = false;
        }
        else if( GLFW_KEY_A == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera.actionLeft = true;
          else if( GLFW_RELEASE == aAction )
            state->camera.actionLeft = false;
        }
        else if( GLFW_KEY_D == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera.actionRight = true;
          else if( GLFW_RELEASE == aAction )
            state->camera.actionRight = false;
        }
        else if( GLFW_KEY_Q == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera.actionUp = true;
          else if( GLFW_RELEASE == aAction )
            state->camera.actionUp = false;
        }
        else if( GLFW_KEY_E == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera.actionDown = true;
          else if( GLFW_RELEASE == aAction )
            state->camera.actionDown = false;
        }
        else if( GLFW_KEY_LEFT_SHIFT == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera.speedMul = .4f;
          else if( GLFW_RELEASE == aAction )
            state->camera.speedMul = .2f;
        }
        else if( GLFW_KEY_LEFT_CONTROL == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera.speedMul = .1f;
          else if( GLFW_RELEASE == aAction )
            state->camera.speedMul = .2f;
        }
      }

      // Check spaceship animation controls
      if( GLFW_KEY_F == aKey )
      {
        if( GLFW_PRESS == aAction )
        {
          state->spaceship_controls.moving = true;
          state->spaceship_controls.reset = false;
        }
      }
      else if( GLFW_KEY_R == aKey )
      {
        if( GLFW_PRESS == aAction )
        {
          state->spaceship_controls.moving = false;
          state->spaceship_controls.reset = true;
        }
      }
    }
  }

  void glfw_callback_motion_( GLFWwindow* aWindow, double aX, double aY )
  {
    if( auto* state = static_cast<State_*>(glfwGetWindowUserPointer( aWindow )) )
    {
      if(state->camera.cameraActive)
      {
        auto const dx = float(aX-state->camera.lastX);
        auto const dy = float(aY-state->camera.lastY);

        state->camera.yaw += dx*kMouseSensitivity_;

        // The most duct-tape solution to clamping the camera yaw between
        // 2*pi and -2*pi you have ever seen. And it works apparently!
        if (state->camera.yaw > 0)
          state->camera.yaw = -128.f * kPi_;

        /* No, I don't why this doesn't work.
        if(state->camera.yaw > 2.f * kPi_)
        {
          state->camera.yaw = (-2.f * kPi_) + (state->camera.yaw - 2.f*kPi_);
        }
        else if(state->camera.yaw < -2.f * kPi_)
        {
          state->camera.yaw = (2.f*kPi_) + (state->camera.yaw - 2.f*kPi_);
        }
        */

        state->camera.pitch += dy*kMouseSensitivity_;
        if( state->camera.pitch > kPi_/2.f )
          state->camera.pitch = kPi_/2.f;
        else if( state->camera.pitch < -kPi_/2.f )
          state->camera.pitch = -kPi_/2.f;
      }

      state->camera.lastX = float(aX);
      state->camera.lastY = float(aY);
    }
  }
}

namespace
{
  GLFWCleanupHelper::~GLFWCleanupHelper()
  {
    glfwTerminate();
  }

  GLFWWindowDeleter::~GLFWWindowDeleter()
  {
    if( window )
      glfwDestroyWindow( window );
  }
}

