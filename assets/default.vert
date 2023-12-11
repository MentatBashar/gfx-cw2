#version 430

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iColor;
layout(location = 2) in vec3 iNormal;
layout(location = 3) in vec2 iTexCoord;

layout(location = 0) uniform mat4 uProjCameraWorld;
layout(location = 1) uniform mat3 uNormalMatrix;
layout(location = 13) uniform mat4 uModelWorld;

out vec3 v2fColor;
out vec3 v2fNormal;
out vec2 v2fTexCoord;
out vec3 v2fWorldPos;

void main()
{
  v2fColor = iColor;

  v2fNormal = normalize(uNormalMatrix * iNormal);

  v2fTexCoord = iTexCoord;

  v2fWorldPos = vec3(uModelWorld * vec4(iPosition, 1.0));

  gl_Position = uProjCameraWorld * vec4(iPosition.xyz, 1.0);

}
