#version 460
layout(set = 0, binding = 0) uniform sampler2D frameTexture;

layout(location = 1) in vec2 fragUv;
layout(location = 0) out vec4 fragColor;

void main() {
    vec3 sampledColor = texture(frameTexture, fragUv).rgb;
    fragColor = vec4( sampledColor, 1.0 );
}