#version 450 core
layout(binding = 0) uniform sampler2D uScreen;
in vec2 fUV;
out vec4 oColor;
void main() {
    oColor = texture(uScreen, fUV);
}