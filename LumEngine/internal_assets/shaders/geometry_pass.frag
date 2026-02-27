#version 450 core
layout(binding = LUM_TEX_ALBEDO) uniform sampler2D tAlbedo;
layout(binding = LUM_TEX_NORMAL) uniform sampler2D tNormal;
layout(binding = LUM_TEX_ROUGHNESS) uniform sampler2D tRoughness;
layout(binding = LUM_TEX_METALNESS) uniform sampler2D tMetalness;

struct DirectionalLight {
	vec3 mDirection;
	float mIntensity;
	vec3 mColor;
	float _padding;
};

struct PointLight {

	vec3 mPosition;
	float mIntensity;
	vec3 mColor;
	float mRadius;

};

layout(std140, binding = LUM_UBO_CAMERA_BINDING) uniform CameraUniforms {
	mat4 LUM_CAMERA_VIEW_MAT4;
	mat4 LUM_CAMERA_PROJECTION_MAT4;
	vec3 LUM_CAMERA_POSITION;
};

layout(std430, binding = LUM_SSBO_LIGHTS_BINDING) readonly buffer LightBuffer {
	
	PointLight mPointLights[LUM_MAX_LIGHTS];
	int mActiveLights;

} aLightBuffer;

layout(std140, binding = LUM_UBO_MATERIAL_BINDING) uniform MaterialUniforms {
	vec3 matBaseColor;
	float matRoughness;
	float matMetallic;
};

vec3 FresnelSchlick(float cosTheta, vec3 F0){
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
float DistriburionGXX(vec3 N, vec3 H, float roughness){

	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = 3.14159265359 * denom * denom;

	return a2 / denom;

}
float GeometrySchlickGXX(float NdotV, float roughness){

	float r = roughness + 1.0;
	float k = (r*r) / 8.0;
	return NdotV / (NdotV * (1.0 - k) + k);

}
float GeometrySmitch(vec3 N, vec3 V, vec3 L, float roughness){

	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGXX(NdotV, roughness);
	float ggx2 = GeometrySchlickGXX(NdotL, roughness);
	return ggx1 * ggx2;

}
vec3 CookTorrance(vec3 N, vec3 V, vec3 L, float roughness, vec3 F0){

	vec3 H = normalize(V + L);

	float D = DistriburionGXX(N, H, roughness);
	float G = GeometrySmitch(N, V, L, roughness);
	vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 numerator = D * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	return numerator / denominator;

}

in vec3 fPos;
in vec2 fUV;
in vec3 fN;
in mat3 fTBN;

out vec4 FinalColor;

void main() {

	vec3 albedo = texture(tAlbedo, fUV).rgb * matBaseColor;
	vec3 normalMap = texture(tNormal, fUV).rgb;

	vec3 Lo = vec3(0.0);

	vec3 V = normalize(LUM_CAMERA_POSITION - fPos);

	normalMap = normalMap * 2.0 - 1.0;
	vec3 N = normalize(fTBN * normalMap);

	vec3 F0 = mix(vec3(0.04), albedo, matMetallic);

	for(uint i = 0; i < aLightBuffer.mActiveLights; i++) {

		PointLight light = aLightBuffer.mPointLights[i];

		vec3 L = normalize(light.mPosition - fPos);
		vec3 H = normalize(V + L);
		float NdotL = max(dot(N, L), 0.0);

		vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 kS = F; // ile energii idzie w specular
		vec3 kD = (1.0 - kS) * (1.0 - matMetallic);

		vec3 specular = CookTorrance(N, V, L, matRoughness, F0);
		vec3 diffuse = (albedo / 3.14159265359) * kD;

		float dist = length(light.mPosition - fPos);
    	float attenuation = clamp(1.0 - dist / light.mRadius, 0.0, 1.0);

		Lo += (diffuse + specular) * NdotL * light.mColor * light.mIntensity * attenuation;

	}

	vec3 ambient = vec3(0.03) * albedo;
    Lo = Lo + ambient;
    Lo = Lo / (Lo + vec3(1.0));        // tonemapping
    Lo = pow(Lo, vec3(1.0 / 2.2));     // gamma correction

	FinalColor = vec4(Lo, 1.0);

}