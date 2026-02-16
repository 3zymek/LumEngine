#version 450 core

struct PointLight {
	vec3 mPosition;
	float _padding1;
	vec3 mColor;
	float mIntensity;
};


layout(binding = LUM_TEX_ALBEDO) uniform sampler2D tAlbedo;
layout(binding = LUM_TEX_NORMAL) uniform sampler2D tNormal;
layout(binding = LUM_TEX_ROUGHNESS) uniform sampler2D tRoughness;
layout(binding = LUM_TEX_METALNESS) uniform sampler2D tMetalness;

layout(std140, binding = LUM_SSBO_LIGHTS_BINDING) readonly buffer LightBuffer {
	PointLight lights[LUM_MAX_LIGHTS];
} aLightBuffer;

in vec3 fPos;
in vec2 fUV;
in vec3 fNormal;

out vec4 FinalColor;

void main() {
	

	FinalColor = texture(tAlbedo, fUV);

}