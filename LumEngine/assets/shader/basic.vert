#version 450 core
layout(location = LUM_LAYOUT_POSITION) in vec3 aPos;
layout(location = LUM_LAYOUT_COLOR) in vec3 aColor;
layout(location = LUM_LAYOUT_UV) in vec2 aUV;

out vec3 fragColor;
out vec2 fragUV;

layout(std140, binding = LUM_UBO_CAMERA_BINDING) uniform CameraUniforms {
	mat4 LUM_VIEW_MAT4;
	mat4 LUM_PROJECTION_MAT4;
};

uniform mat4 LUM_MODEL_MAT4;

void main() {
	gl_Position =
		LUM_PROJECTION_MAT4 * 
		LUM_VIEW_MAT4 * 
		LUM_MODEL_MAT4 * 
		vec4(aPos, 1.0);
	
	fragColor = aColor;
	fragUV = aUV;
}