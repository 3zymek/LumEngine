#version 450 core

layout(location = LUM_LAYOUT_POSITION) in vec3 aPos;

layout(std140, binding = LUM_UBO_CAMERA_BINDING) uniform CameraUniforms {

	mat4 uCameraView;
	mat4 uCameraProj;
	mat4 uCameraInvViewProj;
	vec3 uCameraPos;

};

out vec3 V;

void main( ) {
	
	V = aPos;

	mat4 view = mat4(mat3(uCameraView));

	vec4 pos = uCameraProj * view * vec4(aPos, 1.0);

	gl_Position = pos.xyww;
}