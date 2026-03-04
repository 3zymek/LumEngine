#version 450 core
layout( binding = LUM_TEX_ALBEDO	) uniform sampler2D tAlbedo;
layout( binding = LUM_TEX_NORMAL	) uniform sampler2D tNormal;
layout( binding = LUM_TEX_ROUGHNESS	) uniform sampler2D tRoughness;
layout( binding = LUM_TEX_METALNESS	) uniform sampler2D tMetalness;

layout( location = LUM_GBUFFER_ALBEDO ) out vec4 gAlbedo;
layout( location = LUM_GBUFFER_NORMAL ) out vec4 gNormal;

layout( std140, binding = LUM_UBO_CAMERA_BINDING ) uniform CameraUniforms {
	
	mat4 uCameraView;
	mat4 uCameraProj;
	mat4 uCameraInvProj;
	vec4 uCameraPos;

};

layout( std140, binding = LUM_UBO_MATERIAL_BINDING ) uniform MaterialUniforms {

	vec4 uMatBaseColor;
	float uMatRoughness;
	float uMatMetallic;

};

in vec3 fPos;
in vec2 fUV;
in mat3 fTBN;

void main( ) {

	vec4 albedoTex = texture(tAlbedo, fUV);
	vec3 albedo = albedoTex.rgb * uMatBaseColor.rgb;

	float roughness = texture(tRoughness, fUV).r * uMatRoughness;
	float metallic = texture(tMetalness, fUV).r * uMatMetallic;

	vec3 normal = texture(tNormal, fUV).rgb;

	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(normal * fTBN);

	gAlbedo = vec4(albedo, roughness);
	gNormal = vec4(normal, metallic);

}