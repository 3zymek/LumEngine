#version 450 core

layout(binding = LUM_CUBEMAP_BINDING) uniform samplerCube skybox;

in vec3 viewDir;
out vec4 finalColor;

void main() {
	
	finalColor = texture(skybox, viewDir);

}
