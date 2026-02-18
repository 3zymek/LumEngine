#version 450 core

layout(binding = LUM_TEX_CUBEMAP) uniform samplerCube uCubemap;

in vec3 V;
out vec4 finalColor;

void main() {
	
	finalColor = texture(uCubemap, V);

}
