#version 450 core
layout( binding = LUM_GBUFFER_ALBEDO    ) uniform sampler2D gAlbedo;
layout( binding = LUM_GBUFFER_NORMAL    ) uniform sampler2D gNormal;
layout( binding = LUM_GBUFFER_DEPTH     ) uniform sampler2D gDepth;
layout( binding = LUM_SHADOW_MAP		) uniform sampler2D tShadowMap;

struct FPointLight {

	vec3 mPosition;
	float mIntensity;
	vec3 mColor;
	float mRadius;

};

struct FSpotLight {

    vec3 mPosition;
    float mIntensity;

    vec3 mDirection;
    float mRange;

    vec3 mColor;
    float mInnerCone;
    float mOuterCone;

    float _mPad[3];

};
struct FLightningContext {

    vec3 albedo;
    vec3 N;
    vec3 V;
    vec3 F0;
    float roughness;
    float metallic;
    vec3 fPos;

};

layout( std140, binding = LUM_UBO_CAMERA_BINDING ) uniform CameraUniforms {
	
	mat4 uCameraView;
	mat4 uCameraProj;
	mat4 uCameraInvViewProj;
	vec4 uCameraPos;

};

layout( std430, binding = LUM_SSBO_LIGHTS_BINDING ) readonly buffer LightBuffer {
	
	int uActivePointLights;
	int uActiveSpotLights;
	FPointLight uPointLights[LUM_MAX_LIGHTS];
	FSpotLight uSpotLights[LUM_MAX_LIGHTS];

} aLightBuffer;

layout( std140, binding = LUM_UBO_DIRECTIONAL_LIGHT ) uniform DirectionalLight {

	vec4 mDirDirection;
	vec4 mDirColor;
	float mDirIntensity;

};
layout(std140, binding = LUM_UBO_LIGHTSPACE_MATRIX) uniform LightSpaceMatrix {

    mat4 lightSpaceMatrix;

};

//======================================
// PBR
//======================================

vec3 FresnelSchlick( float cosTheta, vec3 F0 ) {
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
float DistributionGGX( vec3 N, vec3 H, float roughness ) {

	float a = max(roughness * roughness, 0.001);
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

	float D = DistributionGGX(N, H, roughness);
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


//======================================
// Shadows
//======================================

float CalculateShadow( FLightningContext ctx ) {
	
	vec3 L = normalize(-mDirDirection.xyz);

	vec4 lightSpaceFrag = lightSpaceMatrix * vec4(ctx.fPos, 1.0);
	lightSpaceFrag /= lightSpaceFrag.w;
	lightSpaceFrag = lightSpaceFrag * 0.5 + 0.5;

	if(lightSpaceFrag.z > 1.0) return 0.0;

	float bias = max(0.005 * (1.0 - dot(ctx.N, L)), 0.005);
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(tShadowMap, 0);

	for(int x = -1; x <= 1; x++){
		for(int y = -1; y <= 1; y++){
			float depth = texture(tShadowMap, lightSpaceFrag.xy + vec2(x, y) * texelSize).r;
			shadow += step(depth + bias, lightSpaceFrag.z);
		}
	}
	shadow /= 9.0;
	shadow = mix(0.0, 0.9, shadow);

	return shadow;

}

void CalculatePointLights( inout vec3 Lo, in FLightningContext ctx ) {

	for(uint i = 0; i < aLightBuffer.uActivePointLights; i++) {

		FPointLight light = aLightBuffer.uPointLights[i];

		vec3 L = normalize(light.mPosition - ctx.fPos);
		vec3 H = normalize(ctx.V + L);
		float NdotL = max(dot(ctx.N, L), 0.0);

		vec3 F = FresnelSchlick(max(dot(H, ctx.V), 0.0), ctx.F0);

		vec3 kS = F;
		vec3 kD = (1.0 - kS) * (1.0 - ctx.metallic);

		vec3 specular = CookTorrance(ctx.N, ctx.V, L, ctx.roughness, ctx.F0);
		vec3 diffuse = (ctx.albedo / LUM_PI) * kD;

		float dist = length(light.mPosition - ctx.fPos);
    	float attenuation = clamp(1.0 - (dist * dist) / (light.mRadius * light.mRadius), 0.0, 1.0);

		Lo += (diffuse + specular) * NdotL * light.mColor * light.mIntensity * attenuation;

	}
}
void CalculateSpotLights(inout vec3 Lo, in FLightningContext ctx){

	for(uint i = 0; i < aLightBuffer.uActiveSpotLights; i++){

		FSpotLight light = aLightBuffer.uSpotLights[i];

		vec3 L = normalize(light.mPosition - ctx.fPos);
		vec3 H = normalize(L + ctx.V);
		float NdotL = max(dot(ctx.N, L), 0.0);

		float theta = dot(-L, normalize(light.mDirection));
		float epsilon = light.mInnerCone - light.mOuterCone;
		float coneAtten = clamp((theta - light.mOuterCone) / epsilon, 0.0, 1.0);

		if(coneAtten <= 0.0) continue;

		float dist = length(light.mPosition - ctx.fPos);
		float attenuation = clamp(1.0 - (dist * dist) / (light.mRange * light.mRange), 0.0, 1.0);

		vec3 F = FresnelSchlick(max(dot(H, ctx.V), 0.0), ctx.F0);

		vec3 kS = F;
		vec3 kD = (1.0 - kS) * (1.0 - ctx.metallic);

		vec3 specular = CookTorrance(ctx.N, ctx.V, L, ctx.roughness, ctx.F0);
		vec3 diffuse = (ctx.albedo / LUM_PI) * kD;

		Lo += (diffuse + specular) * NdotL * light.mColor * light.mIntensity * attenuation * coneAtten;

	}

}
void CalculateDirectionalLight( inout vec3 Lo, in FLightningContext ctx ){

	float shadow = CalculateShadow(ctx);
	vec3 L = normalize(-mDirDirection.xyz);

	vec3 H = normalize(ctx.V + L);
	float NdotL = max(dot(ctx.N, L), 0.0);
	vec3 F = FresnelSchlick(max(dot(H, ctx.V), 0.0), ctx.F0);
	vec3 kS = F;
	vec3 kD = (1.0 - kS) * (1.0 - ctx.metallic);
	vec3 specular = CookTorrance(ctx.N, ctx.V, L, ctx.roughness, ctx.F0);
	vec3 diffuse = (ctx.albedo / LUM_PI) * kD;
	Lo += (1.0 - shadow) * (diffuse + specular) * NdotL * mDirColor.rgb * mDirIntensity;

}

in vec2 fUV;
out vec4 oFinalColor;

void main( ) {

	vec4 albedoTex = texture(gAlbedo, fUV);
	vec3 albedo = albedoTex.rgb;
	float roughness = albedoTex.a;

	vec4 normalTex = texture(gNormal, fUV);
	float metallic = normalTex.a;
	vec3 normalMap = normalTex.rgb;

    float depth = texture(gDepth, fUV).r;
    vec4 ndc = vec4(fUV * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    vec4 worldPos = uCameraInvViewProj * ndc;
    worldPos /= worldPos.w;
    vec3 fPos = worldPos.xyz;

	vec3 Lo = vec3(0.0);

	vec3 V = normalize(uCameraPos.xyz - fPos);
	vec3 N = normalize(normalMap);

	vec3 F0 = mix(vec3(0.04), albedo, metallic);

	FLightningContext context;
	context.albedo = albedo;
	context.N = N;
	context.V = V;
	context.F0 = F0;
	context.roughness = roughness;
	context.metallic = metallic;
	context.fPos = fPos;

	CalculatePointLights( Lo, context );
	CalculateSpotLights( Lo, context );
	CalculateDirectionalLight( Lo, context );

	vec3 ambient = vec3(0.03) * albedo;
    Lo = Lo + ambient;
    Lo = TonemapACES(Lo);
    Lo = GammaCorrection22(Lo);

	oFinalColor = vec4(Lo, 1.0);

}