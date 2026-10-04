#version 450 core

layout(location = LUM_LAYOUT_POSITION) in vec3 aPos;
layout(location = LUM_LAYOUT_NORMAL) in vec3 aNormal;
layout(location = LUM_LAYOUT_UV) in vec2 aUV;

out vec2 fUV;

void main() {
    
    gl_Position = vec4(aPos, 1.0);
    fUV = aUV;

}