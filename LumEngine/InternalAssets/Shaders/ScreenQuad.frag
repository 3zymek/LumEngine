#version 460
layout(set = 0, binding = 0) uniform sampler2D in_FrameTexture;

layout(location = 1) in vec2 f_Uv;
layout(location = 0) out vec4 f_Color;

void main() {
    vec3 sampledColor = texture(in_FrameTexture, f_Uv).rgb;
    f_Color = vec4( sampledColor, 1.0 );
}