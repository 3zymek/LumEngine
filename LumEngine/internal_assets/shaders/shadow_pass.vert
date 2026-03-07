#version 450 core
layout(location = LUM_LAYOUT_POSITION) in vec3 aPos;
layout(std140, binding = LUM_UBO_LIGHTSPACE_MATRIX) uniform LightSpaceMatrix {
    mat4 uLightSpaceMatrix;
};
layout(std140, binding = LUM_UBO_MODEL_BINDING) uniform ModelUniforms {
    mat4 uModelMatrix;
};
void main() {
    gl_Position = uLightSpaceMatrix * uModelMatrix * vec4(aPos, 1.0);
}