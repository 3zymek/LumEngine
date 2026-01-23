#version 450 core
layout(binding = 7) uniform sampler2D uScreenTex;

in vec2 vUv;

out vec4 final;

void main() {
// Reverse
	final.rgb = texture(uScreenTex, vUv).rgb;
	final.a = 1.0;
}