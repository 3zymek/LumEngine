#version 450 core
layout(location = LUM_LAYOUT_POSITION) in vec3 aPos;
layout(location = LUM_LAYOUT_NORMAL) in vec3 aNormal;
layout(location = LUM_LAYOUT_UV) in vec2 aUV;
layout(std140, binding = LUM_UBO_CAMERA_BINDING) uniform CameraUniforms {
	mat4 LUM_CAMERA_VIEW_MAT4;
	mat4 LUM_CAMERA_PROJECTION_MAT4;
	vec3 LUM_CAMERA_POSITION;
};
layout(std140, binding = LUM_UBO_MODEL_BINDING) uniform ModelUniforms {
	mat4 LUM_MODEL_MAT4;
};

out vec3 fragNormal;
out vec2 fragUV;

void main() {

	gl_Position =
		LUM_CAMERA_PROJECTION_MAT4 * 
		LUM_CAMERA_VIEW_MAT4 * 
		LUM_MODEL_MAT4 * 
		vec4(aPos, 1.0);
	

	//gl_Position = vec4(aPos, 1.0);

	fragNormal = aNormal;
	fragUV = aUV;

}