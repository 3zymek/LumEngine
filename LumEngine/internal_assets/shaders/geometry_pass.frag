#version 450 core

struct DirectionalLight {
	vec3 mDirection;
	float mIntensity;
	vec3 mColor;
	float _padding;
};

layout(binding = LUM_TEX_ALBEDO) uniform sampler2D tAlbedo;
layout(binding = LUM_TEX_NORMAL) uniform sampler2D tNormal;
layout(binding = LUM_TEX_ROUGHNESS) uniform sampler2D tRoughness;
layout(binding = LUM_TEX_METALNESS) uniform sampler2D tMetalness;

layout(std140, binding = LUM_UBO_CAMERA_BINDING) uniform CameraUniforms {
	mat4 LUM_CAMERA_VIEW_MAT4;
	mat4 LUM_CAMERA_PROJECTION_MAT4;
	vec3 LUM_CAMERA_POSITION;
};

layout(std430, binding = LUM_SSBO_LIGHTS_BINDING) readonly buffer LightBuffer {
	DirectionalLight light;
	mat4 lightSpaceMatrix;
} aLightBuffer;

layout(std140, binding = LUM_UBO_MATERIAL_BINDING) uniform MaterialUniforms {
	vec3 matBaseColor;
	float matRoughness;
	float matMetallic;
};

in vec3 fPos;
in vec2 fUV;
in vec3 fN;

out vec4 FinalColor;

void main() {

	float shininess = (1.0 - matRoughness) * 128.0;

	vec3 cameraV = normalize(LUM_CAMERA_POSITION - fPos);
	
	vec3 lightDir = normalize(-aLightBuffer.light.mDirection);
	float diff = max(dot(lightDir, fN), 0.0);
	vec3 diffuse = diff * aLightBuffer.light.mIntensity * aLightBuffer.light.mColor;

	vec3 R = reflect(-lightDir, fN);
	float spec = pow(max(dot(cameraV, R), 0.0), shininess);
	vec3 specular = spec * aLightBuffer.light.mIntensity * aLightBuffer.light.mColor;

	vec3 ambient = vec3(0.05);

	vec4 texColor = texture(tAlbedo, fUV);
	vec3 result = (ambient + diffuse + specular) * texColor.rgb;

	FinalColor = vec4(result * matBaseColor, 1.0);

}