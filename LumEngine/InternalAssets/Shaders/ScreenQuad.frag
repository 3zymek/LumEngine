#version 460
layout(set = 0, binding = 0) uniform texture2D u_FrameTexture;
layout(set = 0, binding = 1) uniform sampler u_Sampler;

layout(location = 1) in vec2 f_Uv;
layout(location = 0) out vec4 f_Color;

void main() {
    vec3 sampledColor = texture(sampler2D(u_FrameTexture, u_Sampler), f_Uv).rgb;
    f_Color = vec4( sampledColor, 1.0 );
}