#version 450 core
layout(binding = LUM_TEX_ALBEDO) uniform sampler2D uAlbedo;

in vec3 fragNormal;
in vec2 fragUV;

out vec4 FinalColor;

void main() {
	FinalColor = texture(uAlbedo, fragUV);
}