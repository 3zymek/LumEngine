#version 450 core

struct DirectionalLight {
	vec3 mDirection;
	float mIntensity;
	vec3 mColor;
	float _padding;
};

layout(binding = LUM_TEX_ALBEDO) uniform sampler2D tAlbedo;
layout(binding = LUM_TEX_NORMAL) uniform sampler2D tNormal;
layout(binding = LUM_TEX_ROUGHNESS) uniform sampler2D tRoughness;
layout(binding = LUM_TEX_METALNESS) uniform sampler2D tMetalness;

layout(std140, binding = LUM_UBO_CAMERA_BINDING) uniform CameraUniforms {
	mat4 LUM_CAMERA_VIEW_MAT4;
	mat4 LUM_CAMERA_PROJECTION_MAT4;
	vec3 LUM_CAMERA_POSITION;
};

layout(std430, binding = LUM_SSBO_LIGHTS_BINDING) readonly buffer LightBuffer {
	DirectionalLight light;
	mat4 lightSpaceMatrix;
} aLightBuffer;

layout(std140, binding = LUM_UBO_MATERIAL_BINDING) uniform MaterialUniforms {
	vec3 matBaseColor;
	float matRoughness;
	float matMetallic;
};

in vec3 fPos;
in vec2 fUV;
in vec3 fN;

out vec4 FinalColor;

vec3 calcDirectionalLight( DirectionalLight light, vec3 N, vec3 V ){

	vec3 I = normalize(-light.mDirection);

	vec3 ambient = vec3( 0.2, 0.2, 0.2 );
	float diff = max(dot(N, I), 0.0);

	vec3 R = reflect(-I, N);

	float spec = pow(max(dot(V, R), 0.0), 10.0);

	vec3 result = ambient + diff * vec3(1.0) + spec * vec3(1.0);
	return result;
}

void main() {

	vec3 albedo = texture(tAlbedo, fUV).rgb * matBaseColor;
	
	vec3 N = normalize(fN);

	vec3 V = normalize(LUM_CAMERA_POSITION - fPos);

	vec3 L = normalize(-aLightBuffer.light.mDirection);

	vec3 H = normalize(V + L);

	vec3 F0 = mix(vec3(0.04), albedo, matMetallic);
    float VdotH = max(dot(V, H), 0.0);
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);

	FinalColor = vec4(F, 1.0);

}