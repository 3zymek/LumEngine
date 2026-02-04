#version 450 core

layout(location = LUM_LAYOUT_POSITION) in vec3 aPos;

layout(std140, binding = LUM_UBO_CAMERA_BINDING) uniform CameraUniforms {
	mat4 LUM_CAMERA_VIEW_MAT4;
	mat4 LUM_CAMERA_PROJECTION_MAT4;
	vec3 LUM_CAMERA_POSITION;
};

out vec3 V;

void main() {
	
	V = aPos;

	mat4 view = mat4(mat3(LUM_CAMERA_VIEW_MAT4));

	vec4 pos = LUM_CAMERA_PROJECTION_MAT4 * view * vec4(aPos, 1.0);

	gl_Position = pos.xyww;
}