#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aUv;
layout(binding = 0) uniform sampler2D tex;

out vec3 vertexColor;

void main() {
	gl_Position = vec4(aPos, 1.0);
	vertexColor = texture(tex, aUv).xyz;
}
