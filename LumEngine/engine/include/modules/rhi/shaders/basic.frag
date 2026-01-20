#version 450 core
layout(binding = LUM_TEXTURE_BINDING_01) uniform sampler2D uTexture;

in vec3 fragColor;
in vec2 fragUV;

in vec3 modelPosition;
in vec3 modelScale;
in vec3 modelRotation;

out vec4 FinalColor;

void main() {
	FinalColor = texture(uTexture, fragUV);
}