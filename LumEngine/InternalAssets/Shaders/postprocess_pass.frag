#version 450 core
layout(binding = LUM_TEX_FRAME) uniform sampler2D uScreen;
layout(binding = LUM_TEX_FRAME_HISTORY) uniform sampler2D uHistory;

layout(location = 0) uniform vec2 uJitterOffset;

in vec2 fUV;
out vec4 oColor;

vec3 TonemapACES( vec3 color ) {
	return clamp((color*(2.51 * color + 0.03)) / (color * (2.43 * color + 0.59) + 0.14), 0.0, 1.0);
}
vec3 GammaCorrection22( vec3 color ) {
	return pow(color, vec3(1.0/2.2));
}

void main( ) {
    
    vec2 stabilizedUV = fUV - uJitterOffset;
    vec3 currentHDR = texture(uScreen, stabilizedUV).rgb;

    vec3 rawHistory = texture(uHistory, fUV).rgb;
    
    vec3 blendHDR = mix(rawHistory, currentHDR, 0.1);
    
    oColor = vec4(blendHDR, 1.0);

}