#version 450 core
layout(binding = LUM_TEX_CUBEMAP) uniform samplerCube uCubemap;
layout(binding = LUM_GBUFFER_DEPTH) uniform sampler2D gDepth;
layout(std140, binding = LUM_UBO_CAMERA_BINDING) uniform CameraUniforms {
    mat4 uCameraView;
    mat4 uCameraProj;
    mat4 uCameraInvViewProj;
    vec3 uCameraPos;
};

in vec3 fPos;
out vec4 oFinalColor;

void main() {

	vec2 screenSize = textureSize(gDepth, 0).xy;
	vec2 screenUV = gl_FragCoord.xy / screenSize; 
    float depth = texture(gDepth, screenUV).r;
    if (depth < 0.9999) discard;

    oFinalColor = texture(uCubemap, fPos);

}