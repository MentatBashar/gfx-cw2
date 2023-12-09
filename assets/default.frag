#version 430

in vec3 v2fColor;
in vec3 v2fNormal;
in vec2 v2fTexCoord;

layout(location = 0) out vec3 oColor;
layout(location = 2) uniform vec3 uLightDir;
layout(location = 3) uniform vec3 uLightDiffuse;
layout(location = 4) uniform vec3 uSceneAmbient;
layout(location = 5) uniform bool uUseTexture;

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
    vec3 normal = normalize(v2fNormal);
    float nDotL = max(0.0, dot(normal, uLightDir));
    vec3 diffuse = nDotL * uLightDiffuse;
    vec3 baseColor;

    // Need to know if using texture
    if (uUseTexture) 
    {
        baseColor = texture(uTexture, v2fTexCoord).rgb;
    } 
    else 
    {
        baseColor = v2fColor;
    }

    oColor = (uSceneAmbient + diffuse) * baseColor;
}
