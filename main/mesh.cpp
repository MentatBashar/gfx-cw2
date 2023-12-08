#include "mesh.hpp"

MeshData mergeMeshes(std::vector<MeshData> const meshes)
{
  MeshData newMesh;

  for (auto& mesh : meshes)
  {
    newMesh.positions.insert(newMesh.positions.end(), mesh.positions.begin(), mesh.positions.end() );
    newMesh.colors.insert(newMesh.colors.end(), mesh.colors.begin(), mesh.colors.end() );
    newMesh.normals.insert(newMesh.normals.end(), mesh.normals.begin(), mesh.normals.end() );
  }

  return newMesh;
}

MeshData transformMesh(MeshData mesh, Mat44f aTransform)
{
  for (auto& p : mesh.positions)
  {
    Vec4f p4{p.x, p.y, p.z, 1.f};
    Vec4f t = aTransform * p4;
    t /= t.w;

    p = Vec3f{t.x, t.y, t.z};
  }

  Mat33f const N = mat44_to_mat33(transpose(invert(aTransform)));
  for (auto& n : mesh.normals)
  {
    n = normalize(N * n);
  }

  return mesh;
}


GLuint create_vao( MeshData const& aMeshData )
{
  GLuint positionVBO = 0;
  glGenBuffers(1, &positionVBO);
  glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
  glBufferData(GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof(Vec3f), aMeshData.positions.data(), GL_STATIC_DRAW);

  GLuint colorVBO = 0;
  glGenBuffers(1, &colorVBO);
  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glBufferData(GL_ARRAY_BUFFER, aMeshData.colors.size() * sizeof(Vec3f), aMeshData.colors.data(), GL_STATIC_DRAW);

  GLuint normalVBO = 0;
  glGenBuffers(1, &normalVBO);
  glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
  glBufferData(GL_ARRAY_BUFFER, aMeshData.normals.size() * sizeof(Vec3f), aMeshData.normals.data(), GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
  glVertexAttribPointer(
      0,
      3, GL_FLOAT, GL_FALSE,
      0,
      0
      );
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glVertexAttribPointer(
      1,
      3, GL_FLOAT, GL_FALSE,
      0,
      0
      );
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
  glVertexAttribPointer(
      2,
      3, GL_FLOAT, GL_FALSE,
      0,
      0
      );
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDeleteBuffers(1, &positionVBO);
  glDeleteBuffers(1, &colorVBO);
  glDeleteBuffers(1, &normalVBO);

  return vao;
}

