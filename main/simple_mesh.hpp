#ifndef MESH_HPP
#define MESH_HPP

#include <glad.h>

#include <vector>

#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"
#include "../vmlib/mat33.hpp"

struct SimpleMeshData
{
  std::vector<Vec3f> positions;
  std::vector<Vec3f> colors;
  std::vector<Vec3f> normals;
};

SimpleMeshData mergeMeshes(std::vector<SimpleMeshData> const meshes);
SimpleMeshData scaleMesh(SimpleMeshData mesh, Mat44f aScaleTransform);

GLuint create_vao( SimpleMeshData const& );

#endif // MESH_HPP
