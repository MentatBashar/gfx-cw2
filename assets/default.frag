#version 430

in vec3 v2fColor;
in vec3 v2fNormal;
in vec2 v2fTexCoord;

layout(location = 0) out vec3 oColor;
layout(location = 2) uniform vec3 uLightDir;
layout(location = 3) uniform vec3 uLightDiffuse;
layout(location = 4) uniform vec3 uSceneAmbient;

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
  vec3 normal = normalize(v2fNormal);

  float nDotL = max(0.0, dot(normal, uLightDir));
  
  vec3 diffuse = nDotL * uLightDiffuse;

  vec3 textureColor = texture(uTexture, v2fTexCoord).rgb;
  oColor = (uSceneAmbient + diffuse) * textureColor * v2fColor;

  // oColor = (uSceneAmbient + nDotL * uLightDiffuse) * v2fColor;
  // oColor = texture( uTexture, v2fTexCoord ).rgb;
  // oColor = normal;
}
