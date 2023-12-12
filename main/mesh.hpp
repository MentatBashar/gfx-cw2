#ifndef MESH_HPP
#define MESH_HPP

#include <glad.h>

#include <vector>

#include "../vmlib/vec2.hpp"
#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"
#include "../vmlib/mat33.hpp"

struct MeshData
{
  std::vector<Vec3f> positions;
  std::vector<Vec3f> colors;
  std::vector<Vec3f> normals;
  std::vector<Vec2f> texcoords;
};

MeshData mergeMeshes(std::vector<MeshData> const meshes);
MeshData transformMesh(MeshData mesh, Mat44f aTransform);

GLuint create_vao(MeshData const&);
GLuint create_point_vao(std::vector<Vec3f> pointData, Vec3f color);

#endif // MESH_HPP
