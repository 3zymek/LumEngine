#version 450 core
layout(binding = 0) uniform sampler2D uTexture;

in vec3 fragColor;
in vec2 fragUV;

out vec4 FinalColor;

void main() {
	FinalColor = texture(uTexture, fragUV);
	//FinalColor = vec4(fragColor, 1.0);
}