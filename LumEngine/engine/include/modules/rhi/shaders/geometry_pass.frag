#version 450 core
layout(binding = LUM_TEX_ALBEDO) uniform sampler2D tAlbedo;
layout(binding = LUM_TEX_NORMAL) uniform sampler2D tNormal;
layout(binding = LUM_TEX_ROUGHNESS) uniform sampler2D tRoughness;
layout(binding = LUM_TEX_METALNESS) uniform sampler2D tMetalness;

in vec3 fragPos;
in vec2 fragUv;
in vec3 fragNormal;

out vec4 FinalColor;

void main() {
	FinalColor = texture(tAlbedo, fragUv);
}