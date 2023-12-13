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
      int mode;

      bool
        actionForward,
        actionBackward,
        actionLeft,
        actionRight,
        actionUp,
        actionDown;

      float speedMul;

      Vec3f pos;

      float yaw, pitch;

      float lastX, lastY;
    } camera_a, camera_b;

    struct SpaceshipControls_
    {
      bool moving, reset;
      Vec3f pos;
    }spaceship_controls;

    Camera_* activeCamera;
    double lastPrintTime = 0.0; // Last print time for cam pos
    uint viewCount = 1;
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

  // Global GL setup goes here
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

  state.activeCamera           = &state.camera_b;
  state.activeCamera->mode     = 0;
  state.activeCamera->pos      = {25.f, 5.f, -10.f};
  state.activeCamera->pitch    = 0.f;
  state.activeCamera->yaw      = kPi_ / -2.f;
  state.activeCamera->speedMul = 0.2f;

  state.activeCamera           = &state.camera_a;
  state.activeCamera->mode     = 0;
  state.activeCamera->pos      = {25.f, 5.f, -10.f};
  state.activeCamera->pitch    = 0.f;
  state.activeCamera->yaw      = kPi_ / -2.f;
  state.activeCamera->speedMul = 0.2f;

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

  // Point lights
  Vec3f pointLightPositions[3] = {
    {25.0f,   .2f, -6.0f},
    {25.2f, -.82f, -6.0f},
    {24.7f, -.72f, -6.0f}
  };
  Vec3f pointLightDiffuseColors[3] = {
    {1.0f,    0.0f,   0.0f},
    {0.0f,    1.0f,   0.0f},
    {0.0f,    0.0f,   1.0f}
  };
  Vec3f pointLightSpecularColors[3] = {
    {1.0f,    1.0f,   0.0f},
    {0.0f,    1.0f,   1.0f},
    {1.0f,    1.0f,   1.0f}
  };

  OGL_CHECKPOINT_ALWAYS();

  float spaceship_clock = 0.f;

  // std::chrono::high_resolution_clock CPU_timer;    // For perf. tests

  // Main loop
  while( !glfwWindowShouldClose( window ) )
  {
    // auto const frame_start_time = CPU_timer.now();   // For perf. tests

    // Let GLFW process events
    glfwPollEvents();


    // Update state
    auto const now = Clock::now();
    float dt = std::chrono::duration_cast<Secondsf>(now-last).count();
    last = now;


    // Update: move spaceship
    if (state.spaceship_controls.moving == true)
    {
      spaceship_clock += dt;
      spaceship_mesh = move_spaceship(spaceship_mesh,
          spaceship_clock,
          &state.spaceship_controls.pos,
          pointLightPositions);
    }
    if (state.spaceship_controls.reset == true)
    {
      spaceship_clock = 0.f;
      spaceship_mesh = make_spaceship();
      state.spaceship_controls.pos = {25.f, -0.77f, -6.f};
      pointLightPositions[0] = {25.0f,   .2f, -6.0f};
      pointLightPositions[1] = {25.2f, -.82f, -6.0f};
      pointLightPositions[2] = {24.7f, -.72f, -6.0f};

      state.spaceship_controls.reset = false;
    }
    spaceship_vao = create_vao(spaceship_mesh);


    for (uint i = 0; i < state.viewCount; ++i)
    {
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
        if (state.viewCount == 1)
        {
          state.activeCamera = &state.camera_a;
          glViewport(0, 0, nwidth, nheight);
        }
        else if (state.viewCount == 2)
        {
          if (i == 0)
          {
            state.activeCamera = &state.camera_a;
            glViewport(0, 0, nwidth/2, nheight);
          }
          else if (i == 1)
          {
            state.activeCamera = &state.camera_b;
            state.activeCamera->cameraActive = state.camera_a.cameraActive;
            glViewport(nwidth/2, 0, nwidth/2, nheight);
          }
        }
      }


      // Update activeCamera state
      if(state.activeCamera->actionForward)
      { 
        float yaw = state.activeCamera->yaw;
        yaw += kPi_ / 2.f;
        if (yaw > 2.f * kPi_)
          yaw = -2.f * kPi_ + (2.f*kPi_ - yaw);

        float pitch = state.activeCamera->pitch;

        state.activeCamera->pos.x -= cos(yaw) * cos(pitch) * state.activeCamera->speedMul;
        state.activeCamera->pos.y -= sin(pitch) * state.activeCamera->speedMul;
        state.activeCamera->pos.z -= sin(yaw) * cos(pitch) * state.activeCamera->speedMul;
      }
      else if(state.activeCamera->actionBackward)
      {
        float yaw = state.activeCamera->yaw;
        yaw += kPi_ / 2.f;
        if (yaw > 2.f * kPi_)
          yaw = -2.f * kPi_ + (2.f*kPi_ - yaw);

        float pitch = state.activeCamera->pitch;

        state.activeCamera->pos.x += cos(yaw) * cos(pitch) * state.activeCamera->speedMul;
        state.activeCamera->pos.y += sin(pitch) * state.activeCamera->speedMul;
        state.activeCamera->pos.z += sin(yaw) * cos(pitch) * state.activeCamera->speedMul;
      }
      else if(state.activeCamera->actionLeft)
      {
        float yaw = state.activeCamera->yaw;
        state.activeCamera->pos.x -= cos(yaw) * state.activeCamera->speedMul;
        state.activeCamera->pos.z -= sin(yaw) * state.activeCamera->speedMul;
      }
      else if(state.activeCamera->actionRight)
      {
        float yaw = state.activeCamera->yaw;
        state.activeCamera->pos.x += cos(yaw) * state.activeCamera->speedMul;
        state.activeCamera->pos.z += sin(yaw) * state.activeCamera->speedMul;
      }
      else if(state.activeCamera->actionUp)
      {
        float yaw = state.activeCamera->yaw;
        float pitch = state.activeCamera->pitch;

        state.activeCamera->pos.x += sin(yaw) * sin(pitch) * state.activeCamera->speedMul;
        state.activeCamera->pos.y += cos(pitch) * state.activeCamera->speedMul;
        state.activeCamera->pos.z -= cos(yaw) * sin(pitch) * state.activeCamera->speedMul;
      }
      else if(state.activeCamera->actionDown)
      {
        float yaw = state.activeCamera->yaw;
        float pitch = state.activeCamera->pitch;

        state.activeCamera->pos.x -= sin(yaw) * sin(pitch) * state.activeCamera->speedMul;
        state.activeCamera->pos.y -= cos(pitch) * state.activeCamera->speedMul;
        state.activeCamera->pos.z += cos(yaw) * sin(pitch) * state.activeCamera->speedMul;
      }

      // Fixed-distance activeCamera
      if (state.activeCamera->mode == 1)
      {
        state.activeCamera->pos = state.spaceship_controls.pos;

        state.activeCamera->pos.y += 5.f;
        state.activeCamera->pos.z -= 10.f;
      }
      // Tracking activeCamera
      else if (state.activeCamera->mode == 2)
      {
        // Work out yaw
        state.activeCamera->yaw = -1.f * atan(
            (state.activeCamera->pos.x - state.spaceship_controls.pos.x) /
            (state.activeCamera->pos.z - state.spaceship_controls.pos.z)
            );

        // Work out pitch
      state.activeCamera->pitch = atan(
          (state.activeCamera->pos.y - state.spaceship_controls.pos.y) /
          (state.activeCamera->pos.z - state.spaceship_controls.pos.z)
          );
    }

      // Update: compute matrices
      Mat44f model2world = make_rotation_y(angle);

      Mat44f Rx = make_rotation_x(state.activeCamera->pitch);
      Mat44f Ry = make_rotation_y(state.activeCamera->yaw);

      Mat44f T = make_translation({-state.activeCamera->pos.x,
                                   -state.activeCamera->pos.y,
                                   -state.activeCamera->pos.z});
      Mat44f world2activeCamera = Rx * Ry * T;

      float aspect_ratio;
      if (state.viewCount == 1)
        aspect_ratio = fbwidth/float(fbheight);
      else
        aspect_ratio = (fbwidth/2.f)/float(fbheight);

      Mat44f projection = make_perspective_projection(
          60.f * kPi_ / 180.f,
          aspect_ratio,
          0.1f, 200.f
          );

      Mat44f projCameraWorld = projection * world2activeCamera * model2world;

      Mat33f normalMatrix = mat44_to_mat33(transpose(invert(model2world)));

      // Draw scene
      OGL_CHECKPOINT_DEBUG();

      unsigned int full_render_time_query_ids[2];
      glGenQueries(2, full_render_time_query_ids);

      unsigned int terrain_render_time_query_ids[2];
      glGenQueries(2, terrain_render_time_query_ids);

      unsigned int spaceship_render_time_query_ids[2];
      glGenQueries(2, spaceship_render_time_query_ids);

      unsigned int landing_pad_render_time_query_ids[2];
      glGenQueries(2, landing_pad_render_time_query_ids);

      // ------------------------- BEGIN RENDER TIME ---------------------------

      glQueryCounter(full_render_time_query_ids[0], GL_TIMESTAMP);


      // ------------------------------- DRAW FRAME ----------------------------

      if (i == 0)
      {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      }

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

      glQueryCounter(terrain_render_time_query_ids[0], GL_TIMESTAMP);

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

      glQueryCounter(terrain_render_time_query_ids[1], GL_TIMESTAMP);

      // ------------------------------- SPACE SHIP -------------------------------

      glQueryCounter(spaceship_render_time_query_ids[0], GL_TIMESTAMP);

      glBindVertexArray(spaceship_vao);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glDrawArrays(GL_TRIANGLES, 0, spaceshipVertexCount);

      glQueryCounter(spaceship_render_time_query_ids[1], GL_TIMESTAMP);

      // ------------------------------- POINT LIGHTS -------------------------------

      for (int i = 0; i < 3; ++i) 
      {
        std::string index = std::to_string(i);
        glUniform3fv(glGetUniformLocation(prog.programId(), ("uPointLightPositions[" + index + "]").c_str()), 1, &pointLightPositions[i].x);
        glUniform3fv(glGetUniformLocation(prog.programId(), ("uPointLightDiffuse[" + index + "]").c_str()), 1, &pointLightDiffuseColors[i].x);
        glUniform3fv(glGetUniformLocation(prog.programId(), ("uPointLightSpecular[" + index + "]").c_str()), 1, &pointLightSpecularColors[i].x);
      }


      // ------------------------------- LANDING PAD -------------------------------

      glQueryCounter(landing_pad_render_time_query_ids[0], GL_TIMESTAMP);

      // Light for landing pad
      // Mat44f landingpadModelMatrix = kIdentity44f;
      //glUniformMatrix4fv(13, 1, GL_TRUE, landingpadModelMatrix.v);

      //Vec3f landingpadSpecularColor = { 0.f, 0.f, 1.f };
      //glUniform3fv(6, 1, &landingpadSpecularColor.x);
      float landingpadShininess = 32.0f;
      glUniform1f(7, landingpadShininess);

      Mat44f model1 = landingpadTransform1; 
      Mat44f projCameraWorld1 = projection * world2activeCamera * model1;
      glUniformMatrix4fv(0, 1, GL_TRUE, projCameraWorld1.v);
      glBindVertexArray(landingpad_vao);
      glDrawArrays(GL_TRIANGLES, 0, landingpadVertexCount);

      Mat44f model2 = landingpadTransform2;
      Mat44f projCameraWorld2 = projection * world2activeCamera * model2;
      glUniformMatrix4fv(0, 1, GL_TRUE, projCameraWorld2.v);
      glBindVertexArray(landingpad_vao);
      glDrawArrays(GL_TRIANGLES, 0, landingpadVertexCount);

      glQueryCounter(landing_pad_render_time_query_ids[1], GL_TIMESTAMP);

      // ------------------------- END RENDER TIME --------------------------

      glQueryCounter(full_render_time_query_ids[1], GL_TIMESTAMP);

      GLuint64 full_render_start_time, full_render_stop_time,
               terrain_render_start_time, terrain_render_stop_time,
               spaceship_render_start_time, spaceship_render_stop_time,
               landing_pad_render_start_time, landing_pad_render_stop_time;

      glGetQueryObjectui64v(full_render_time_query_ids[0],
          GL_QUERY_RESULT,
          &full_render_start_time);
      glGetQueryObjectui64v(full_render_time_query_ids[1],
          GL_QUERY_RESULT,
          &full_render_stop_time);

      glGetQueryObjectui64v(terrain_render_time_query_ids[0],
          GL_QUERY_RESULT,
          &terrain_render_start_time);
      glGetQueryObjectui64v(terrain_render_time_query_ids[1],
          GL_QUERY_RESULT,
          &terrain_render_stop_time);

      glGetQueryObjectui64v(spaceship_render_time_query_ids[0],
          GL_QUERY_RESULT,
          &spaceship_render_start_time);
      glGetQueryObjectui64v(spaceship_render_time_query_ids[1],
          GL_QUERY_RESULT,
          &spaceship_render_stop_time);

      glGetQueryObjectui64v(landing_pad_render_time_query_ids[0],
          GL_QUERY_RESULT,
          &landing_pad_render_start_time);
      glGetQueryObjectui64v(landing_pad_render_time_query_ids[1],
          GL_QUERY_RESULT,
          &landing_pad_render_stop_time);

/*
      std::printf("Terrain render time: %.6f ms\n",
          (terrain_render_stop_time - terrain_render_start_time) / 1000000.f);
      std::printf("Spaceship render time: %.6f ms\n",
          (spaceship_render_stop_time - spaceship_render_start_time) / 1000000.f);
      std::printf("Landing pad render time: %.6f ms\n",
          (landing_pad_render_stop_time - landing_pad_render_start_time) / 1000000.f);
      std::printf("Full render time: %.6f ms\n\n",
          (full_render_stop_time - full_render_start_time) / 1000000.f);
*/

/*    TO BE REDIRECTED TO FILE OUTPUT
 
      std::printf("%.6f,",
          (terrain_render_stop_time - terrain_render_start_time) / 1000000.f);
      std::printf("%.6f,",
          (spaceship_render_stop_time - spaceship_render_start_time) / 1000000.f);
      std::printf("%.6f,",
          (landing_pad_render_stop_time - landing_pad_render_start_time) / 1000000.f);
      std::printf("%.6f,,,\n",
          (full_render_stop_time - full_render_start_time) / 1000000.f);
*/

      OGL_CHECKPOINT_DEBUG();
    }


    // ------------------------------- DEBUG PRINTS -------------------------------

/*
    double currentTime = glfwGetTime();

    // Print cam position help to place landing pad
    if (currentTime - state.lastPrintTime >= 1.0)
    {
      std::printf("Camera Position: X = %.2f, Y = %.2f, Z = %.2f\n",
          state.activeCamera->pos.x, state.activeCamera->pos.y, state.activeCamera->pos.z);
      std::printf("Spaceship Position: X = %.2f, Y = %.2f, Z = %.2f\n",
          state.spaceship_controls.pos.x,
          state.spaceship_controls.pos.y,
          state.spaceship_controls.pos.z);
      state.lastPrintTime = currentTime;
    }
*/
    // Display results
    glfwSwapBuffers( window );
/*
    auto const frame_end_time = CPU_timer.now();
    float CPU_frame_time = std::chrono::duration_cast<Secondsf>(frame_end_time-frame_start_time).count();
    std::printf("CPU frame render time: %.6f ms\n\n", CPU_frame_time * 1000);
*/
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

  void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int aMod)
  {
    if( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction )
    {
      glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
      return;
    }

    if( auto* state = static_cast<State_*>(glfwGetWindowUserPointer( aWindow )) )
    {
      // Space toggles activeCamera
      if( GLFW_KEY_SPACE == aKey && GLFW_PRESS == aAction )
      {
        state->camera_a.cameraActive = !state->camera_a.cameraActive;
        state->camera_b.cameraActive = !state->camera_b.cameraActive;

        if( state->activeCamera->cameraActive )
          glfwSetInputMode( aWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
        else
          glfwSetInputMode( aWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
      }

      // Change number of views
      if( GLFW_KEY_V == aKey )
      {
        if( GLFW_PRESS == aAction )
        {
          if (state->viewCount == 1)
            state->viewCount = 2;
          else if (state->viewCount == 2)
            state->viewCount = 1;
        }
      }

      // Change activeCamera mode
      if(GLFW_KEY_C == aKey && GLFW_MOD_SHIFT != aMod)
      {
        if( GLFW_PRESS == aAction )
        {
          state->camera_a.mode++;
          if (state->camera_a.mode >= 3)
            state->camera_a.mode = 0;
          if (state->camera_a.mode == 1)
          {
            state->camera_a.yaw = kPi_;
            state->camera_a.pitch = 0.464f;
          }
          else if (state->camera_a.mode == 2)
          {
            state->camera_a.pos = {21.04f, 2.75f, 1.44f};
          }
        }
      }
      if(GLFW_KEY_C == aKey && GLFW_MOD_SHIFT == aMod)
      {
        if( GLFW_PRESS == aAction )
        {
          state->camera_b.mode++;
          if (state->camera_b.mode >= 3)
            state->camera_b.mode = 0;
          if (state->camera_b.mode == 1)
          {
            state->camera_b.yaw = kPi_;
            state->camera_b.pitch = 0.464f;
          }
          else if (state->camera_b.mode == 2)
          {
            state->camera_b.pos = {21.04f, 2.75f, 1.44f};
          }
        }
      }

      // Camera controls if camera_a is active
      if(state->camera_a.cameraActive && state->camera_a.mode == 0)
      {
        // First-person mode
        if( GLFW_KEY_W == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_a.actionForward = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_a.actionForward = false;
        }
        else if( GLFW_KEY_S == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_a.actionBackward = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_a.actionBackward = false;
        }
        else if( GLFW_KEY_A == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_a.actionLeft = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_a.actionLeft = false;
        }
        else if( GLFW_KEY_D == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_a.actionRight = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_a.actionRight = false;
        }
        else if( GLFW_KEY_Q == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_a.actionUp = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_a.actionUp = false;
        }
        else if( GLFW_KEY_E == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_a.actionDown = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_a.actionDown = false;
        }
        else if( GLFW_KEY_LEFT_SHIFT == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_a.speedMul = .4f;
          else if( GLFW_RELEASE == aAction )
            state->camera_a.speedMul = .2f;
        }
        else if( GLFW_KEY_LEFT_CONTROL == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_a.speedMul = .1f;
          else if( GLFW_RELEASE == aAction )
            state->camera_a.speedMul = .2f;
        }
      }
      // Camera controls if camera_b is active
      if(state->camera_b.cameraActive && state->camera_b.mode == 0)
      {
        // First-person mode
        if( GLFW_KEY_W == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_b.actionForward = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_b.actionForward = false;
        }
        else if( GLFW_KEY_S == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_b.actionBackward = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_b.actionBackward = false;
        }
        else if( GLFW_KEY_A == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_b.actionLeft = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_b.actionLeft = false;
        }
        else if( GLFW_KEY_D == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_b.actionRight = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_b.actionRight = false;
        }
        else if( GLFW_KEY_Q == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_b.actionUp = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_b.actionUp = false;
        }
        else if( GLFW_KEY_E == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_b.actionDown = true;
          else if( GLFW_RELEASE == aAction )
            state->camera_b.actionDown = false;
        }
        else if( GLFW_KEY_LEFT_SHIFT == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_b.speedMul = .4f;
          else if( GLFW_RELEASE == aAction )
            state->camera_b.speedMul = .2f;
        }
        else if( GLFW_KEY_LEFT_CONTROL == aKey )
        {
          if( GLFW_PRESS == aAction )
            state->camera_b.speedMul = .1f;
          else if( GLFW_RELEASE == aAction )
            state->camera_b.speedMul = .2f;
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
      if(state->camera_a.cameraActive && state->camera_a.mode == 0)
      {
        auto const dx = float(aX-state->camera_a.lastX);
        auto const dy = float(aY-state->camera_a.lastY);

        state->camera_a.yaw += dx*kMouseSensitivity_;

        // The most duct-tape solution to clamping the activeCamera yaw between
        // 2*pi and -2*pi you have ever seen. And it works apparently!
        if (state->camera_a.yaw > 0)
          state->camera_a.yaw = -128.f * kPi_;

        /* No, I don't why this doesn't work.
           if(state->camera_a.yaw > 2.f * kPi_)
           {
           state->camera_a.yaw = (-2.f * kPi_) + (state->camera_a.yaw - 2.f*kPi_);
           }
           else if(state->camera_a.yaw < -2.f * kPi_)
           {
           state->camera_a.yaw = (2.f*kPi_) + (state->camera_a.yaw - 2.f*kPi_);
           }
        */

        state->camera_a.pitch += dy*kMouseSensitivity_;
        if( state->camera_a.pitch > kPi_/2.f )
          state->camera_a.pitch = kPi_/2.f;
        else if( state->camera_a.pitch < -kPi_/2.f )
          state->camera_a.pitch = -kPi_/2.f;

      }
      if(state->camera_b.cameraActive && state->camera_b.mode == 0)
      {
        auto const dx = float(aX-state->camera_b.lastX);
        auto const dy = float(aY-state->camera_b.lastY);

        state->camera_b.yaw += dx*kMouseSensitivity_;

        // The most duct-tape solution to clamping the activeCamera yaw between
        // 2*pi and -2*pi you have ever seen. And it works apparently!
        if (state->camera_b.yaw > 0)
          state->camera_b.yaw = -128.f * kPi_;

        /* No, I don't why this doesn't work.
           if(state->camera_b.yaw > 2.f * kPi_)
           {
           state->camera_b.yaw = (-2.f * kPi_) + (state->camera_b.yaw - 2.f*kPi_);
           }
           else if(state->camera_b.yaw < -2.f * kPi_)
           {
           state->camera_b.yaw = (2.f*kPi_) + (state->camera_b.yaw - 2.f*kPi_);
           }
        */

        state->camera_b.pitch += dy*kMouseSensitivity_;
        if( state->camera_b.pitch > kPi_/2.f )
          state->camera_b.pitch = kPi_/2.f;
        else if( state->camera_b.pitch < -kPi_/2.f )
          state->camera_b.pitch = -kPi_/2.f;

      }
        state->camera_a.lastX = float(aX);
        state->camera_a.lastY = float(aY);
        state->camera_b.lastX = float(aX);
        state->camera_b.lastY = float(aY);
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
