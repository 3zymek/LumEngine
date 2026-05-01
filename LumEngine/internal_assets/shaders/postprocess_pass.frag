#version 450 core
layout(binding = LUM_TEX_FRAME) uniform sampler2D uScreen;
layout(binding = LUM_TEX_FRAME_HISTORY) uniform sampler2D uHistory;

in vec2 fUV;
out vec4 oColor;

vec3 TonemapACES( vec3 color ) {
	return clamp((color*(2.51 * color + 0.03)) / (color * (2.43 * color + 0.59) + 0.14), 0.0, 1.0);
}
vec3 GammaCorrection22( vec3 color ) {
	return pow(color, vec3(1.0/2.2));
}
vec3 TemporalAA( vec3 color ) {
    vec3 history = texture(uHistory, fUV).rgb;
    return mix(history, color, 0.5);
}

void main( ) {
    
    vec3 color = texture(uScreen, fUV).rgb;
    color *= 0.4;
    color = TemporalAA(color);
    color = TonemapACES(color);
    color = GammaCorrection22(color);

    oColor = vec4(color, 1.0);

}