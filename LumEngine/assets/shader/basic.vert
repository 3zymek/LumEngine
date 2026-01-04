#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aUv;

layout(std140, binding = 0) uniform uVP {
	mat4 uProjection;
	mat4 uView;
};

uniform mat4 uModel;
out vec3 vertexPos;
out vec3 vertexColor;

void main() {
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
	vertexPos = vec3(uModel * vec4(aPos, 1.0));
	vertexColor = aColor;
}
