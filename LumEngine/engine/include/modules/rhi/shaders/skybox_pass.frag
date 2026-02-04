#version 450 core

layout(binding = LUM_CUBEMAP_BINDING) uniform samplerCube skybox;

in vec3 V;
out vec4 finalColor;

void main() {
	
	finalColor = texture(skybox, V);

}
