#version 450 core
layout(location = LUM_LAYOUT_POSITION) in vec3 aPosition;
layout(location = LUM_LAYOUT_UV) in vec2 aUV;

out vec2 fUV;

void main() {
    fUV = aUV;
    gl_Position = vec4(aPosition, 1.0);
}