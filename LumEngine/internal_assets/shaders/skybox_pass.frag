#version 450 core
layout(binding = LUM_TEX_CUBEMAP) uniform samplerCube uCubemap;
layout(binding = LUM_GBUFFER_DEPTH) uniform sampler2D gDepth;
layout(std140, binding = LUM_UBO_CAMERA_BINDING) uniform CameraUniforms {
    mat4 uCameraView;
    mat4 uCameraProj;
    mat4 uCameraInvViewProj;
    vec3 uCameraPos;
};

vec3 ACESTonemapping( in vec3 c) {
    return clamp((c * ( 2.51*c+0.03))/(c*(2.43*c+0.59)+0.14), 0.0, 1.0);
}

in vec3 fPos;
out vec4 oFinalColor;

void main() {

	vec2 screenSize = textureSize(gDepth, 0).xy;
	vec2 screenUV = gl_FragCoord.xy / screenSize; 
    float depth = texture(gDepth, screenUV).r;
    if (depth < 0.9999) discard;

    oFinalColor = vec4(ACESTonemapping(texture(uCubemap, fPos).rgb), 1.0);

}