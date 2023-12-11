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
layout(location = 9) uniform vec3 uLightPos;            // Position of Point light
layout(location = 11) uniform vec3 uPointLightDiffuse;  // LightDiffuse color of point light
layout(location = 12) uniform vec3 uPointLightSpecular; // LightSpecular color of point light

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
    vec3 normal = normalize(v2fNormal);
     vec3 viewDir = normalize(uCameraPos - v2fWorldPos);
    //vec3 viewDir = normalize(uCameraPos - gl_FragCoord.xyz);
    vec3 baseColor = uUseTexture ? texture(uTexture, v2fTexCoord).rgb : v2fColor;

    // Handle Directional Lights "Sun"
    vec3 dirLightDir = normalize(uLightDir);
    float diffDir = max(dot(normal, dirLightDir), 0.0);
    vec3 reflectDirDir = reflect(-dirLightDir, normal);
    float specDir = pow(max(dot(viewDir, reflectDirDir), 0.0), uShininess);
    vec3 dirDiffuse = diffDir * uLightDiffuse;
    vec3 dirSpecular = specDir * uLightSpecular;

    // Handle Directional Point Lights
    vec3 pointLightDir = normalize(uLightPos - v2fWorldPos);
    float dist = length(uLightPos - v2fWorldPos);
    float a = 1.0;  
    float b = 0.09; 
    float c = 0.032;
    float attenuation = 1.0 / (a + b * dist + c * dist * dist);
    
    float diffPoint = max(dot(normal, pointLightDir), 0.0);
    vec3 reflectDirPoint = reflect(-pointLightDir, normal);
    float specPoint = pow(max(dot(viewDir, reflectDirPoint), 0.0), uShininess);
    vec3 pointDiffuse = diffPoint * uPointLightDiffuse * attenuation;
    vec3 pointSpecular = specPoint * uPointLightSpecular * attenuation;

    // Final Color
    vec3 finalDiffuse = dirDiffuse + pointDiffuse;
    vec3 finalSpecular = dirSpecular + pointSpecular;
    oColor = (uSceneAmbient + finalDiffuse + finalSpecular) * baseColor;
}
