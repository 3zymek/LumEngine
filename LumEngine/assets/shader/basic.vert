#version 440 core
layout(location = LUM_POSITION) in vec3 aPos;
layout(location = LUM_COLOR) in vec3 aColor;
layout(location = LUM_UV) in vec2 aUV;

out vec2 fragUV;

uniform mat4 LUM_PROJECTION_MAT4;
uniform mat4 LUM_VIEW_MAT4;
uniform mat4 LUM_MODEL_MAT4;

void main() {
	gl_Position = vec4(aPos, 1.0);
	fragUV = aUV;
}