#version 450
layout(location = LUM_LAYOUT_POSITION) in vec3 aPos;

layout(std430, binding = LUM_SSBO_LIGHTS_BINDING) readonly buffer LightBuffer {
	DirectionalLight light;
	mat4 lightSpaceMatrix;
} aLightBuffer;

layout(std140, binding = LUM_UBO_MODEL_BINDING) uniform ModelUniforms {
	mat4 LUM_MODEL_MAT4;
};

void main() {

}