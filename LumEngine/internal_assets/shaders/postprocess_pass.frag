#version 450 core
layout(binding = 0) uniform sampler2D uScreen;

vec3 TonemapACES(vec3 color){
	return clamp((color*(2.51 * color + 0.03)) / (color * (2.43 * color + 0.59) + 0.14), 0.0, 1.0);
}
vec3 GammaCorrection22(vec3 color){
	return pow(color, vec3(1.0/2.2));
}

in vec2 fUV;
out vec4 oColor;

void main( ) {
    
    vec3 color = texture(uScreen, fUV).rgb;
    color *= 0.2;
    color = TonemapACES(color);
    color = GammaCorrection22(color);
    oColor = vec4(color, 1.0);

}