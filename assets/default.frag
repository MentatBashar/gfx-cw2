#version 430

in vec3 v2fColor;
in vec3 v2fNormal;
in vec2 v2fTexCoord;
in vec3 v2fWorldPos;

layout(location = 0) out vec3 oColor;
layout(location = 2) uniform vec3 uLightDir;            // Direction for direction light "sun"
layout(location = 3) uniform vec3 uLightDiffuse;        // LightDiffuse color for "sun"
layout(location = 4) uniform vec3 uSceneAmbient;        // SceneAmbient color for "sun"
layout(location = 5) uniform bool uUseTexture;          // Use texture or not
layout(location = 6) uniform vec3 uLightSpecular;       // LightSpecular color for "sun"
layout(location = 7) uniform float uShininess;          // Shininess
layout(location = 8) uniform vec3 uCameraPos;           // Camera Position

// Uniform arrays for point lights
uniform vec3 uPointLightPositions[3];
uniform vec3 uPointLightDiffuse[3];
uniform vec3 uPointLightSpecular[3];

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
    vec3 normal = normalize(v2fNormal);
    vec3 viewDir = normalize(uCameraPos - v2fWorldPos);
    //vec3 viewDir = normalize(uCameraPos - gl_FragCoord.xyz);
    vec3 baseColor = uUseTexture ? texture(uTexture, v2fTexCoord).rgb : v2fColor;

    // Handle Directional Lights "Sun"
    // Don't need spec lights for directional light
    vec3 dirLightDir = normalize(uLightDir);
    float diffDir = max(dot(normal, dirLightDir), 0.0);
    vec3 reflectDirDir = reflect(-dirLightDir, normal);
    // float specDir = pow(max(dot(viewDir, reflectDirDir), 0.0), uShininess);
    vec3 dirDiffuse = diffDir * uLightDiffuse;
    // vec3 dirSpecular = specDir * uLightSpecular;

    // Handle point lights
    // Init value
    vec3 pointDiffuse = vec3(0.0);
    vec3 pointSpecular = vec3(0.0);

    for(int i = 0; i < 3; ++i) 
    {
        vec3 pointLightDir = normalize(uPointLightPositions[i] - v2fWorldPos);
        float diffPoint = max(dot(normal, pointLightDir), 0.0);
        vec3 reflectDirPoint = reflect(-pointLightDir, normal);
        float specPoint = pow(max(dot(viewDir, reflectDirPoint), 0.0), uShininess);

        float dist = length(uPointLightPositions[i] - v2fWorldPos);
        float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);

        pointDiffuse += diffPoint * uPointLightDiffuse[i] * attenuation;
        pointSpecular += specPoint * uPointLightSpecular[i] * attenuation;
    }

    // Final Color
    vec3 finalDiffuse = dirDiffuse + pointDiffuse;
    vec3 finalSpecular = pointSpecular;
    oColor = (uSceneAmbient + finalDiffuse + finalSpecular) * baseColor;
}
