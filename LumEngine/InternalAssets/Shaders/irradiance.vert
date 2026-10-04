#version 450 core
layout(binding = LUM_UBO_IRRADIANCE) uniform IrradianceCapture {
    mat4 uProj;
    mat4 uView;
};

layout(location = LUM_LAYOUT_POSITION) in vec3 aPos;

out vec3 fPos;

void main() {
    fPos = aPos;
    gl_Position = uProj * uView * vec4(aPos, 1.0);
}