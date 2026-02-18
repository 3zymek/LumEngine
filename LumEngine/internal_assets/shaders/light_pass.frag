#version 450 core

// Inputs z vertex shadera
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Output
out vec4 FragColor;

// Uniforms
uniform vec3 viewPos;
uniform sampler2D albedoMap;

// Œwiat³a - UBO
layout(std140, binding = 1) uniform Lights {
    vec4 positionAndRadius[16];  // xyz = pos, w = radius
    vec4 colorAndIntensity[16];  // rgb = color, a = intensity
} lights;

void main() {
    // Material
    vec3 albedo = texture(albedoMap, TexCoord).rgb;
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Ambient
    vec3 result = vec3(0.03) * albedo;
    
    // Loop przez wszystkie œwiat³a
    for (int i = 0; i < 16; i++) {
        float intensity = lights.colorAndIntensity[i].a;
        
        // Jeœli intensity == 0, to koniec œwiate³
        if (intensity == 0.0) break;
        
        vec3 lightPos = lights.positionAndRadius[i].xyz;
        float radius = lights.positionAndRadius[i].w;
        vec3 lightColor = lights.colorAndIntensity[i].rgb;
        
        // Light direction & distance
        vec3 lightDir = lightPos - FragPos;
        float distance = length(lightDir);
        lightDir = normalize(lightDir);
        
        // Attenuation
        float attenuation = 1.0 / (1.0 + distance * distance / (radius * radius));
        attenuation = clamp(attenuation, 0.0, 1.0);
        
        // Diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = lightColor * diff * albedo;
        
        // Specular (Blinn-Phong)
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        vec3 specular = lightColor * spec * 0.5;
        
        // Add to result
        result += (diffuse + specular) * intensity * attenuation;
    }
    
    // Gamma correction
    result = pow(result, vec3(1.0/2.2));
    
    FragColor = vec4(result, 1.0);
}