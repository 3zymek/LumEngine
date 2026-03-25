#version 450 core

layout(binding = LUM_TEX_CUBEMAP) uniform samplerCube uEnvMap;

in vec3 fPos;
out vec4 oFinalColor;

void main( ) {
    
    vec3 N = normalize(fPos);

    vec3 up = abs(N.y) < 0.999 ? vec3(0, 1, 0) : vec3(1, 0, 0);
    vec3 right = normalize(cross(up, N));
    up = cross(N, right);

    vec3 irradiance = vec3(0.0);
    float sampleCount = 0.0;

    float sampleDelta = 0.025;
    // copied from claude bc WHAT IN THE FUCK IS THIS
    for (float phi = 0.0; phi < 2.0 * LUM_PI; phi += sampleDelta) {
        for (float theta = 0.0; theta < 0.5 * LUM_PI; theta += sampleDelta) {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
            irradiance += texture(uEnvMap, sampleVec).rgb * cos(theta) * sin(theta);
            sampleCount++;
        }
    }
    
    irradiance = LUM_PI * irradiance / sampleCount;
    oFinalColor = vec4(1.0, 0.0, 0.0, 1.0);

}