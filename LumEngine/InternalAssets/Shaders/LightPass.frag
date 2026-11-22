#version 450 core
layout( set = 0, binding = 1 ) uniform sampler2D u_Albedo;
layout( set = 0, binding = 2 ) uniform sampler2D u_Depth;
//layout( binding = LUM_GBUFFER_ALBEDO    ) uniform sampler2D gAlbedo;
//layout( binding = LUM_GBUFFER_NORMAL    ) uniform sampler2D gNormal;
//layout( binding = LUM_GBUFFER_DEPTH     ) uniform sampler2D gDepth;
//layout( binding = LUM_SHADOW_MAP        ) uniform sampler2D tShadowMap;
//layout( binding = LUM_TEX_IRRADIANCE    ) uniform samplerCube tIrradianceMap;
//layout( binding = LUM_TEX_PREFILTERED    ) uniform samplerCube tPrefilteredMap;
/*
struct PointLight {

	vec3 m_Position;
	float m_Intensity;
	vec3 m_Color;
	float m_Radius;

};

struct SpotLight {

    vec3 mPosition;
    float mIntensity;

    vec3 mDirection;
    float mRange;

    vec3 mColor;
    float mInnerCone;
    float mOuterCone;

    float _mPad[3];

};
*/

// SHADER ONLY
struct LightningContext {

    vec3 m_Albedo;
    vec3 m_N;
    vec3 m_V;
    vec3 m_F0;
    float m_Roughness;
    float m_Metalness;
    vec3 m_FragPos;

};

layout( set = 0, binding = 0, std140 ) uniform CameraUniforms {
	mat4 m_View;
	mat4 m_Projection;
	mat4 m_InvertedProjection;
	vec4 m_Position;
} u_CameraData;

/*
layout( set = 0, binding = 3, std430 ) readonly buffer LightBuffer {
	
	PointLight uPointLights[100];
	SpotLight uSpotLights[100];
	int uActivePointLights;
	int uActiveSpotLights;

} u_LightBuffer;
*/

layout( set = 0, binding = 3, std140 ) uniform DirectionalLight {

	vec4 m_Direction;
	vec4 m_Color;
	float m_Intensity;

} u_DirectionalLight;

/*
layout(std140, binding = LUM_UBO_LIGHTSPACE_MATRIX) uniform LightSpaceMatrix {

    mat4 lightSpaceMatrix;

};
*/

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
	denom = 3.141549 * denom * denom;

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


//======================================
// Shadows
//======================================
/*
float CalculateShadow( LightningContext ctx ) {
	
	vec3 L = normalize(-mDirDirection.xyz);

	vec4 lightSpaceFrag = lightSpaceMatrix * vec4(ctx.fPos, 1.0);
	lightSpaceFrag /= lightSpaceFrag.w;
	lightSpaceFrag = lightSpaceFrag * 0.5 + 0.5;

	if(lightSpaceFrag.z > 1.0) return 0.0;

	float bias = max(0.005 * (1.0 - dot(ctx.N, L)), 0.0005);
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(tShadowMap, 0);

	for(int x = -2; x <= 2; x++){
		for(int y = -2; y <= 2; y++){
			float depth = texture(tShadowMap, lightSpaceFrag.xy + vec2(x, y) * texelSize).r;
			shadow += step(depth + bias, lightSpaceFrag.z);
		}
	}
	shadow /= 25.0;
	shadow = mix(0.0, 0.999, shadow);

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
*/
void CalculateDirectionalLight( inout vec3 Lo, in LightningContext ctx ) {

	//float shadow = CalculateShadow(ctx);
	vec3 L = normalize(-u_DirectionalLight.m_Direction.xyz);

	vec3 H = normalize(ctx.m_V + L);
	float NdotL = max(dot(ctx.m_N, L), 0.0);
	vec3 F = FresnelSchlick(max(dot(H, ctx.m_V), 0.0), ctx.m_F0);
	vec3 kS = F;
	vec3 kD = (1.0 - kS) * (1.0 - ctx.m_Metalness);
	vec3 specular = CookTorrance(ctx.m_N, ctx.m_V, L, ctx.m_Roughness, ctx.m_F0);

	vec3 diffuse = (ctx.m_Albedo / 3.141549) * kD;
	Lo += (diffuse + specular) * NdotL * u_DirectionalLight.m_Color.rgb * u_DirectionalLight.m_Intensity;

}

layout( location = 0 ) in vec2 f_UV;
layout( location = 0 ) out vec4 o_Color;

void main( ) {

	vec3 albedo = texture(u_Albedo, f_UV).rgb;
	float depth = texture(u_Depth, f_UV).r;

	vec4 ndc = vec4(f_UV * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
	vec4 worldPos = u_CameraData.m_InvertedProjection * ndc;
	worldPos /= worldPos.w;
	vec3 fragPos = worldPos.xyz;

	vec3 lo = vec3(0.0);
	vec3 f0 = mix(vec3(0.03), albedo, 1.0 /* <- IN FUTURE METALNESS */);

	LightningContext context;
	context.m_Albedo = albedo;
	context.m_N = vec3(0.0, 0.0, 1.0);
	context.m_V = normalize(u_CameraData.m_Position.xyz - fragPos);
	context.m_F0 = f0;
	context.m_Roughness = 0.5;
	context.m_Metalness = 0.0;
	context.m_FragPos = fragPos;

	CalculateDirectionalLight(lo, context);

	o_Color = vec4(lo, 1.0); 

/*
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

	vec3 R = reflect(-context.V, context.N);
	vec3 prefilteredColor = textureLod(tPrefilteredMap, R, context.roughness * 4.0).rgb;
	vec3 F = FresnelSchlick(max(dot(context.N, context.V), 0.0), context.F0);
	vec3 specularIBL = prefilteredColor * F;

	vec3 kS = F;
	vec3 kD = (1.0 - kS) * (1.0 - context.metallic);
	vec3 diffuseIBL = texture(tIrradianceMap, context.N).rgb * context.albedo * kD;

	Lo += diffuseIBL + specularIBL;

	oFinalColor = vec4(Lo, 1.0);
*/

}
