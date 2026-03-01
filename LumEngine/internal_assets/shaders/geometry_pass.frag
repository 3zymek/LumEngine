#version 450 core
layout( binding = LUM_TEX_ALBEDO	) uniform sampler2D tAlbedo;
layout( binding = LUM_TEX_NORMAL	) uniform sampler2D tNormal;
layout( binding = LUM_TEX_ROUGHNESS	) uniform sampler2D tRoughness;
layout( binding = LUM_TEX_METALNESS	) uniform sampler2D tMetalness;

struct PointLight {

	vec3 mPosition;
	float mIntensity;
	vec3 mColor;
	float mRadius;

};

layout( std140, binding = LUM_UBO_CAMERA_BINDING ) uniform CameraUniforms {
	
	mat4 uCameraView;
	mat4 uCameraProj;
	vec4 uCameraPos;

};

layout( std430, binding = LUM_SSBO_LIGHTS_BINDING ) readonly buffer LightBuffer {
	
	PointLight uPointLights[LUM_MAX_LIGHTS];
	int uActiveLights;

} aLightBuffer;

layout( std140, binding = LUM_UBO_DIRECTIONAL_LIGHT ) uniform DirectionalLight {

	vec4 mDirDirection;
	vec4 mDirColor;
	float mDirIntensity;

};


layout( std140, binding = LUM_UBO_MATERIAL_BINDING ) uniform MaterialUniforms {

	vec4 uMatBaseColor;
	float uMatRoughness;
	float uMatMetallic;

};

vec3 FresnelSchlick( float cosTheta, vec3 F0 ) {
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
float DistributionGXX( vec3 N, vec3 H, float roughness ) {

	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = LUM_PI * denom * denom;

	return a2 / denom;

}
float GeometrySchlickGGX( float NdotV, float roughness ) {

	float r = roughness + 1.0;
	float k = (r*r) / 8.0;
	return NdotV / (NdotV * (1.0 - k) + k);

}
float GeometrySmith( vec3 N, vec3 V, vec3 L, float roughness ) {

	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);
	return ggx1 * ggx2;

}
vec3 CookTorrance( vec3 N, vec3 V, vec3 L, float roughness, vec3 F0 ) {

	vec3 H = normalize(V + L);

	float D = DistributionGXX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 numerator = D * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	return numerator / denominator;

}
vec3 TonemapACES(vec3 color){
	return clamp((color*(2.51 * color + 0.03)) / (color * (2.43 * color + 0.59) + 0.14), 0.0, 1.0);
}
vec3 GammaCorrection22(vec3 color){
	return pow(color, vec3(1.0/2.2));
}

in vec3 fPos;
in vec2 fUV;
in mat3 fTBN;

out vec4 oFinalColor;

void main( ) {

	vec3 albedo = texture(tAlbedo, fUV).rgb * uMatBaseColor.rgb;
	vec3 normalMap = texture(tNormal, fUV).rgb;

	vec3 Lo = vec3(0.0);

	normalMap = normalMap * 2.0 - 1.0;
	vec3 V = normalize(uCameraPos.xyz - fPos);
	vec3 N = normalize(fTBN * normalMap);
	vec3 F0 = mix(vec3(0.04), albedo, uMatMetallic);

	for(uint i = 0; i < aLightBuffer.uActiveLights; i++) {

		PointLight light = aLightBuffer.uPointLights[i];

		vec3 L = normalize(light.mPosition - fPos);
		vec3 H = normalize(V + L);
		float NdotL = max(dot(N, L), 0.0);

		vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 kS = F;
		vec3 kD = (1.0 - kS) * (1.0 - uMatMetallic);

		vec3 specular = CookTorrance(N, V, L, uMatRoughness, F0);
		vec3 diffuse = (albedo / LUM_PI) * kD;

		float dist = length(light.mPosition - fPos);
    	float attenuation = clamp(1.0 - (dist * dist) / (light.mRadius * light.mRadius), 0.0, 1.0);

		Lo += (diffuse + specular) * NdotL * light.mColor * light.mIntensity * attenuation;

	}
	
	vec3 L = normalize(-mDirDirection.xyz);
	vec3 H = normalize(V + L);
	float NdotL = max(dot(N, L), 0.0);

	vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 kS = F;
	vec3 kD = (1.0 - kS) * (1.0 - uMatMetallic);

	vec3 specular = CookTorrance(N, V, L, uMatRoughness, F0);
	vec3 diffuse = (albedo / LUM_PI) * kD;

	Lo += (diffuse + specular) * NdotL * mDirColor.rgb * mDirIntensity;

	vec3 ambient = vec3(0.03) * albedo;
    Lo = Lo + ambient;
    Lo = TonemapACES(Lo);
    Lo = GammaCorrection22(Lo);

	oFinalColor = vec4(Lo, 1.0);

}