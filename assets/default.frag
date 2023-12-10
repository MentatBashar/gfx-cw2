#version 430

in vec3 v2fColor;
in vec3 v2fNormal;
in vec2 v2fTexCoord;

layout(location = 0) out vec3 oColor;
layout(location = 2) uniform vec3 uLightDir;
layout(location = 3) uniform vec3 uLightDiffuse;
layout(location = 4) uniform vec3 uSceneAmbient;
layout(location = 5) uniform bool uUseTexture;
layout(location = 6) uniform vec3 uLightSpecular;
layout(location = 7) uniform float uShininess; 
layout(location = 8) uniform vec3 uCameraPos; 
layout(location = 9) uniform vec3 uLightPos;
layout(location = 10) uniform int uLightType; // 0 directional , 1 point light

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
    vec3 normal = normalize(v2fNormal);
    vec3 baseColor;
    float attenuation = 1.0;

  
    if (uLightType == 1) 
    {
        float distance = length(uLightPos - gl_FragCoord.xyz);
        attenuation = 1.0 / (distance * distance);
    }

    // Diffuse
    float nDotL = max(0.0, dot(normal, uLightDir));
    vec3 diffuse = nDotL * uLightDiffuse * attenuation;

    // Specular
    vec3 viewDir = normalize(uCameraPos - gl_FragCoord.xyz);
    vec3 halfDir = normalize(uLightDir + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), uShininess);
    vec3 specular = spec * uLightSpecular * attenuation;

    // Base Color
    if (uUseTexture) 
    {
        baseColor = texture(uTexture, v2fTexCoord).rgb;
    } 
    else 
    {
        baseColor = v2fColor;
    }

    // Output Color
    oColor = (uSceneAmbient + diffuse + specular) * baseColor;
}
