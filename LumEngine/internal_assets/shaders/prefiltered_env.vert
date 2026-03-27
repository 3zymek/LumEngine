#version 450 core

layout(location = LUM_LAYOUT_POSITION) in vec3 aPos;

layout(binding = LUM_UBO_PREFILTERED_MAP) uniform CaptureUniforms {
    mat4 uProj;
    mat4 uView;
    float uRoughness;
};

out vec3 fPos;

void main() {

    fPos = aPos;
    gl_Position = uProj * uView * vec4(aPos, 1.0);

}