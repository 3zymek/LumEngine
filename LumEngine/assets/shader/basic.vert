#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
/*
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aUv;
*/
out vec3 vertexPos;
out vec3 vertexColor;

void main() {
	gl_Position = vec4(aPos, 1.0);
	vertexPos = aPos;
	vertexColor = aColor;
}
