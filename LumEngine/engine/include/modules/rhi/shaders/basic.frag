#version 450 core
layout(binding = LUM_TEXTURE_BINDING_01) uniform sampler2D uTexture;

in vec3 fragColor;
in vec2 fragUV;

out vec4 FinalColor;

void main() {
	FinalColor = vec4(fragColor, 1.0);
	//FinalColor = texture(uTexture, fragUV);
}