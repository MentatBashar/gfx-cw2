#include "mesh.hpp"

MeshData mergeMeshes(std::vector<MeshData> const meshes)
{
  MeshData newMesh;

  for (auto& mesh : meshes)
  {
    newMesh.positions.insert(newMesh.positions.end(), mesh.positions.begin(), mesh.positions.end() );
    newMesh.colors.insert(newMesh.colors.end(), mesh.colors.begin(), mesh.colors.end() );
    newMesh.normals.insert(newMesh.normals.end(), mesh.normals.begin(), mesh.normals.end() );
    newMesh.texcoords.insert(newMesh.texcoords.end(), mesh.texcoords.begin(), mesh.texcoords.end());
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


GLuint create_vao(MeshData const& aMeshData) {
    GLuint positionVBO = 0;
    GLuint colorVBO = 0;
    GLuint normalVBO = 0;
    GLuint texcoordVBO = 0;
    GLuint vao = 0;

    // Create and bind vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Position VBO
    glGenBuffers(1, &positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof(Vec3f), aMeshData.positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Color VBO
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.colors.size() * sizeof(Vec3f), aMeshData.colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Normal VBO
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, aMeshData.normals.size() * sizeof(Vec3f), aMeshData.normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // Texture coord VBO
    // SpaceShip don't have any texture, so we need to add this if. 
    // If not, program will try to read a nullptr
    if (!aMeshData.texcoords.empty()) 
    {
        glGenBuffers(1, &texcoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
        glBufferData(GL_ARRAY_BUFFER, aMeshData.texcoords.size() * sizeof(Vec2f), aMeshData.texcoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(3);
    }

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete
    glDeleteBuffers(1, &positionVBO);
    glDeleteBuffers(1, &colorVBO);
    glDeleteBuffers(1, &normalVBO);
    if (!aMeshData.texcoords.empty()) 
    {
        glDeleteBuffers(1, &texcoordVBO);
    }

    return vao;
}

std::vector<Vec3f> transformPointData (Vec3f newPos){
  
  std::vector<Vec3f> returnPointData = {
    {newPos.x + 0.075f, newPos.y, newPos.z},
    {newPos.x + 0.225f, newPos.y, newPos.z},
    {newPos.x + 0.225f, newPos.y - .15f, newPos.z},
    {newPos.x + 0.075f, newPos.y - .15f, newPos.z},
    {newPos.x + 0.075f, newPos.y, newPos.z},
    {newPos.x + 0.225f, newPos.y - .15f, newPos.z}
  };

  return returnPointData;
}

GLuint create_point_vao(std::vector<Vec3f> pointData, Vec3f color) {
    GLuint positionVBO = 0;
    GLuint colorVBO = 0;
    GLuint vao = 0;

    // Create and bind vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Position VBO
    glGenBuffers(1, &positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, pointData.size() * sizeof(Vec3f), pointData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Color VBO
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), std::vector<Vec3f>{color}.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete
    glDeleteBuffers(1, &positionVBO);
    glDeleteBuffers(1, &colorVBO);

    return vao;
}


