#include <cmath>

#include "spaceship.hpp"
#include <iostream>


MeshData move_spaceship(MeshData spaceship_mesh, float t)
{
  float dy, drZ;

  // dy increases with a quadratic curve
  dy = std::pow(t, 2) / 4096.f;

  // A relatively slow rotation
  drZ = 1.f / 4096.f;

  auto newMesh = transformMesh(spaceship_mesh,
                               make_translation({0.f, dy, 0.f}) *
                               make_rotation_z(drZ));

  return newMesh;
}

MeshData make_spaceship()
{
  auto tank1_body = make_cylinder(true, 128, {1.f, 0.2f, 0.2f},
                                 make_rotation_z(3.141592f / 2.f) *
                                 make_scaling(3.f, .5f, .5f) *
                                 make_translation({0.f, 2.f, 0.f}));
  auto tank1_lower_cone = make_cone(true, 128, {.2f, 1.f, 0.2f},
                                 make_rotation_z(-3.141592f / 2.f) *
                                 make_scaling(.3f, .5f, .5f) *
                                 make_translation({0.f, 2.f, 0.f}));
  auto tank1_upper_cone = make_cone(true, 128, {.2f, 1.f, 0.2f},
                                 make_rotation_z(3.141592f / 2.f) *
                                 make_scaling(.3f, .5f, .5f) *
                                 make_translation({10.f, 2.f, 0.f}));

  auto tank2_body = make_cylinder(true, 128, {1.f, 0.2f, 0.2f},
                                 make_rotation_z(3.141592f / 2.f) *
                                 make_scaling(3.f, .5f, .5f) *
                                 make_translation({0.f, -2.f, 0.f}));
  auto tank2_lower_cone = make_cone(true, 128, {.2f, 1.f, 0.2f},
                                 make_rotation_z(-3.141592f / 2.f) *
                                 make_scaling(.3f, .5f, .5f) *
                                 make_translation({0.f, -2.f, 0.f}));
  auto tank2_upper_cone = make_cone(true, 128, {.2f, 1.f, 0.2f},
                                 make_rotation_z(3.141592f / 2.f) *
                                 make_scaling(.3f, .5f, .5f) *
                                 make_translation({10.f, -2.f, 0.f}));

  auto tank3_body = make_cylinder(true, 128, {1.f, 0.2f, 0.2f},
                                 make_rotation_z(3.141592f / 2.f) *
                                 make_scaling(3.f, .5f, .5f) *
                                 make_translation({0.f, 0.f, 2.f}));
  auto tank3_lower_cone = make_cone(true, 128, {.2f, 1.f, 0.2f},
                                 make_rotation_z(-3.141592f / 2.f) *
                                 make_scaling(.3f, .5f, .5f) *
                                 make_translation({0.f, 0.f, 2.f}));
  auto tank3_upper_cone = make_cone(true, 128, {.2f, 1.f, 0.2f},
                                 make_rotation_z(3.141592f / 2.f) *
                                 make_scaling(.3f, .5f, .5f) *
                                 make_translation({10.f, 0.f, 2.f}));

  auto tank4_body = make_cylinder(true, 128, {1.f, 0.2f, 0.2f},
                                 make_rotation_z(3.141592f / 2.f) *
                                 make_scaling(3.f, .5f, .5f) *
                                 make_translation({0.f, 0.f, -2.f}));
  auto tank4_lower_cone = make_cone(true, 128, {.2f, 1.f, 0.2f},
                                 make_rotation_z(-3.141592f / 2.f) *
                                 make_scaling(.3f, .5f, .5f) *
                                 make_translation({0.f, 0.f, -2.f}));
  auto tank4_upper_cone = make_cone(true, 128, {.2f, 1.f, 0.2f},
                                 make_rotation_z(3.141592f / 2.f) *
                                 make_scaling(.3f, .5f, .5f) *
                                 make_translation({10.f, 0.f, -2.f}));
  

  auto body = make_cube({.2f, .2f, 1.f},
                        make_scaling(.5f, 2.25f, .5f) *
                        make_translation({0.f, .75f, 0.f})
                        );
  auto engine = make_cone(true, 128, {.2f, 1.f, .2f},
                          make_rotation_z(3.141592f / 2.f) *
                          make_scaling(.75f, .75f, .75f) *
                          make_translation({-1.25f, 0.f, 0.f}));

  auto capsule_lower_cone= make_cone(true, 128, {.2f, 1.f, .2f},
                                     make_rotation_z(-3.141592f / 2.f) *
                                     make_scaling(1.2f, .5f, .5f) *
                                     make_translation({-4.f, 0.f, 0.f}));
  auto capsule_upper_cone= make_cone(true, 128, {.2f, 1.f, .2f},
                                     make_rotation_z(3.141592f / 2.f) *
                                     make_scaling(1.2f, .5f, .5f) *
                                     make_translation({4.f, 0.f, 0.f}));

  std::vector<MeshData> meshList = {
    tank1_body,
    tank1_lower_cone,
    tank1_upper_cone,
    tank2_body,
    tank2_lower_cone,
    tank2_upper_cone,
    tank3_body,
    tank3_lower_cone,
    tank3_upper_cone,
    tank4_body,
    tank4_lower_cone,
    tank4_upper_cone,
    body,
    engine,
    capsule_lower_cone,
    capsule_upper_cone
  };

  auto spaceship_mesh = mergeMeshes(meshList);
  
  spaceship_mesh = transformMesh(spaceship_mesh,
                                 make_translation({25.f, -0.77f, -6.f}) *
                                 make_scaling(0.2f, 0.2f, 0.2f)
                                 );

  return spaceship_mesh;
}

MeshData make_cube(Vec3f aColor, Mat44f aPreTransform)
{
  std::vector<Vec3f> pos;
  std::vector<Vec3f> nor;

  std::vector<Vec3f> verts = {
    Vec3f{-1.f, -1.f, -1.f},
    Vec3f{1.f, -1.f, -1.f},
    Vec3f{1.f, 1.f, -1.f},
    Vec3f{-1.f, 1.f, -1.f},
    Vec3f{-1.f, 1.f, 1.f},
    Vec3f{1.f, 1.f, 1.f},
    Vec3f{1.f, -1.f, 1.f},
    Vec3f{-1.f, -1.f, 1.f},
  };

  // this took far too long and my eyes hurt
  pos.emplace_back(verts[0]); // face front
  pos.emplace_back(verts[2]);
  pos.emplace_back(verts[1]);
  pos.emplace_back(verts[0]);
  pos.emplace_back(verts[3]);
  pos.emplace_back(verts[2]);
  pos.emplace_back(verts[2]); // face top
  pos.emplace_back(verts[3]);
  pos.emplace_back(verts[4]);
  pos.emplace_back(verts[2]);
  pos.emplace_back(verts[4]);
  pos.emplace_back(verts[5]);
  pos.emplace_back(verts[1]); // face right
  pos.emplace_back(verts[2]);
  pos.emplace_back(verts[5]);
  pos.emplace_back(verts[1]);
  pos.emplace_back(verts[5]);
  pos.emplace_back(verts[6]);
  pos.emplace_back(verts[0]); // face left
  pos.emplace_back(verts[7]);
  pos.emplace_back(verts[4]);
  pos.emplace_back(verts[0]);
  pos.emplace_back(verts[4]);
  pos.emplace_back(verts[3]);
  pos.emplace_back(verts[5]); // face back
  pos.emplace_back(verts[4]);
  pos.emplace_back(verts[7]);
  pos.emplace_back(verts[5]);
  pos.emplace_back(verts[7]);
  pos.emplace_back(verts[6]);
  pos.emplace_back(verts[0]); // face bottom
  pos.emplace_back(verts[6]);
  pos.emplace_back(verts[7]);
  pos.emplace_back(verts[0]);
  pos.emplace_back(verts[1]);
  pos.emplace_back(verts[6]);

  nor.emplace_back(Vec3f{0.f, 0.f, -1.f}); // face front
  nor.emplace_back(Vec3f{0.f, 0.f, -1.f});
  nor.emplace_back(Vec3f{0.f, 0.f, -1.f});
  nor.emplace_back(Vec3f{0.f, 0.f, -1.f});
  nor.emplace_back(Vec3f{0.f, 0.f, -1.f});
  nor.emplace_back(Vec3f{0.f, 0.f, -1.f});
  nor.emplace_back(Vec3f{0.f, 1.f, 0.f}); // face top
  nor.emplace_back(Vec3f{0.f, 1.f, 0.f});
  nor.emplace_back(Vec3f{0.f, 1.f, 0.f});
  nor.emplace_back(Vec3f{0.f, 1.f, 0.f});
  nor.emplace_back(Vec3f{0.f, 1.f, 0.f});
  nor.emplace_back(Vec3f{0.f, 1.f, 0.f});
  nor.emplace_back(Vec3f{1.f, 0.f, 0.f}); // face right
  nor.emplace_back(Vec3f{1.f, 0.f, 0.f});
  nor.emplace_back(Vec3f{1.f, 0.f, 0.f});
  nor.emplace_back(Vec3f{1.f, 0.f, 0.f});
  nor.emplace_back(Vec3f{1.f, 0.f, 0.f});
  nor.emplace_back(Vec3f{1.f, 0.f, 0.f});
  nor.emplace_back(Vec3f{-1.f, 0.f, 0.f}); // face left
  nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});
  nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});
  nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});
  nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});
  nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});
  nor.emplace_back(Vec3f{0.f, 0.f, 1.f}); // face back
  nor.emplace_back(Vec3f{0.f, 0.f, 1.f});
  nor.emplace_back(Vec3f{0.f, 0.f, 1.f});
  nor.emplace_back(Vec3f{0.f, 0.f, 1.f});
  nor.emplace_back(Vec3f{0.f, 0.f, 1.f});
  nor.emplace_back(Vec3f{0.f, 0.f, 1.f});
  nor.emplace_back(Vec3f{0.f, -1.f, 0.f}); // face bottom
  nor.emplace_back(Vec3f{0.f, -1.f, 0.f});
  nor.emplace_back(Vec3f{0.f, -1.f, 0.f});
  nor.emplace_back(Vec3f{0.f, -1.f, 0.f});
  nor.emplace_back(Vec3f{0.f, -1.f, 0.f});
  nor.emplace_back(Vec3f{0.f, -1.f, 0.f});

  for (auto& p : pos)
  {
    Vec4f p4{p.x, p.y, p.z, 1.f};
    Vec4f t = aPreTransform * p4;
    t /= t.w;

    p = Vec3f{t.x, t.y, t.z};
  }

  std::vector col(pos.size(), aColor);

  Mat33f const N = mat44_to_mat33(transpose(invert(aPreTransform)));
  for (auto& n : nor)
  {
    n = normalize(N * n);
  }

  return MeshData{std::move(pos), std::move(col), std::move(nor)};
}


MeshData make_cylinder( bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform )
{
  std::vector<Vec3f> pos;
  std::vector<Vec3f> nor;

  float prevY = std::cos(0.f);
  float prevZ = std::sin(0.f);

  for (std::size_t i = 0; i < aSubdivs; ++i)
  {
    float const angle = (i+1) / float(aSubdivs) * 2.f * 3.1415926f;

    float y = std::cos(angle);
    float z = std::sin(angle);

    pos.emplace_back(Vec3f{0.f, prevY, prevZ});
    pos.emplace_back(Vec3f{0.f, y, z});
    pos.emplace_back(Vec3f{1.f, prevY, prevZ});

    nor.emplace_back(Vec3f{0, y, z});
    nor.emplace_back(Vec3f{0, y, z});
    nor.emplace_back(Vec3f{0, y, z});

    pos.emplace_back(Vec3f{0.f, y, z});
    pos.emplace_back(Vec3f{1.f, y, z});
    pos.emplace_back(Vec3f{1.f, prevY, prevZ});

    nor.emplace_back(Vec3f{0, y, z});
    nor.emplace_back(Vec3f{0, y, z});
    nor.emplace_back(Vec3f{0, y, z});

    if (aCapped == true)
    {
      pos.emplace_back(Vec3f{0.f, 0.f, 0.f}); 
      pos.emplace_back(Vec3f{0.f, y, z}); 
      pos.emplace_back(Vec3f{0.f, prevY, prevZ});

      nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});
      nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});
      nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});

      pos.emplace_back(Vec3f{1.f, 0.f, 0.f}); 
      pos.emplace_back(Vec3f{1.f, prevY, prevZ}); 
      pos.emplace_back(Vec3f{1.f, y, z});

      nor.emplace_back(Vec3f{1.f, 0.f, 0.f});
      nor.emplace_back(Vec3f{1.f, 0.f, 0.f});
      nor.emplace_back(Vec3f{1.f, 0.f, 0.f});
    }

    prevY = y;
    prevZ = z;
  }

  for (auto& p : pos)
  {
    Vec4f p4{p.x, p.y, p.z, 1.f};
    Vec4f t = aPreTransform * p4;
    t /= t.w;

    p = Vec3f{t.x, t.y, t.z};
  }

  std::vector col(pos.size(), aColor);

  Mat33f const N = mat44_to_mat33(transpose(invert(aPreTransform)));
  for (auto& n : nor)
  {
    n = normalize(N * n);
  }

  return MeshData{std::move(pos), std::move(col), std::move(nor)};
}


MeshData make_cone( bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform )
{
  std::vector<Vec3f> pos;
  std::vector<Vec3f> nor;

  float prevY = std::cos(0.f);
  float prevZ = std::sin(0.f);

  for (std::size_t i = 0; i < aSubdivs; ++i)
  {
    float const angle = (i+1) / float(aSubdivs) * 2.f * 3.1415926f;

    float y = std::cos(angle);
    float z = std::sin(angle);

    pos.emplace_back(Vec3f{1.f, 0.f, 0.f});
    pos.emplace_back(Vec3f{0.f, prevY, prevZ});
    pos.emplace_back(Vec3f{0.f, y, z});

    nor.emplace_back(Vec3f{1.f, 0.f, 0.f});
    nor.emplace_back(Vec3f{0.f, y, z});
    nor.emplace_back(Vec3f{0.f, y, z});

    if (aCapped == true)
    {
      pos.emplace_back(Vec3f{0.f, 0.f, 0.f});
      pos.emplace_back(Vec3f{0.f, y, z});
      pos.emplace_back(Vec3f{0.f, prevY, prevZ});

      nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});
      nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});
      nor.emplace_back(Vec3f{-1.f, 0.f, 0.f});
    }

    prevY = y;
    prevZ = z;
  }

  for (auto& p : pos)
  {
    Vec4f p4{p.x, p.y, p.z, 1.f};
    Vec4f t = aPreTransform * p4;
    t /= t.w;

    p = Vec3f{t.x, t.y, t.z};
  }

  std::vector col(pos.size(), aColor);

  Mat33f const N = mat44_to_mat33(transpose(invert(aPreTransform)));
  for (auto& n : nor)
  {
    n = normalize(N * n);
  }

  return MeshData{std::move(pos), std::move(col), std::move(nor)};
}
