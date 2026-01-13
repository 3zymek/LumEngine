#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aUV;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec3 fragPos;
out vec3 fragColor;
out vec2 fragUV;

void main() {
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
	fragPos = aPos;
	fragColor = aColor;
	fragUV = aUV;
}