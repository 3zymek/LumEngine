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

	vec4 texColor = texture(tAlbedo, fUV);

	FinalColor = vec4(texColor.xyz * matBaseColor, 1.0);

}