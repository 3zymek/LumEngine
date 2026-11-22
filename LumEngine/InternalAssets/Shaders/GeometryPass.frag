#version 460
//layout( set = 0, binding = 2 ) uniform sampler2D in_Albedo;
//layout( binding = LUM_TEX_NORMAL	) uniform sampler2D tNormal;
//layout( binding = LUM_TEX_ROUGHNESS	) uniform sampler2D tRoughness;
//layout( binding = LUM_TEX_METALNESS	) uniform sampler2D tMetalness;

layout( location = 0 ) out vec4 g_Albedo;
//layout( location = LUM_GBUFFER_NORMAL ) out vec4 gNormal;

struct MaterialData {
	vec4 m_BaseColor;
	float m_Roughness;
	float m_Metalness;
};

layout( set = 0, binding = 1, std430 ) readonly buffer MaterialBuffer {
	MaterialData m_Materials[];
} u_MaterialBuffer;

layout( push_constant, std140 ) uniform PushConstants {
	mat4 m_Matrix;
	uint m_MaterialIndex;
	uint _m_Pad0;
	uint _m_Pad1;
	uint _m_Pad2;
} pc_Draw;

layout( location = 0 ) in vec3 f_Position;
layout( location = 1 ) in vec2 f_UV;
//in mat3 fTBN;

void main( ) {

	g_Albedo = u_MaterialBuffer.m_Materials[pc_Draw.m_MaterialIndex].m_BaseColor;

	//vec4 albedo = texture(in_Albedo, f_Uv).rgb; // * uMatBaseColor.rgb;

	//float roughness = texture(tRoughness, fUV).r * uMatRoughness;
	//float metallic = texture(tMetalness, fUV).r * uMatMetallic;
	//if(metallic == 0)
		//metallic = uMatMetallic;

	//vec3 normal = texture(tNormal, fUV).rgb;

	//normal = normalize(normal * 2.0 - 1.0);
	//normal = normalize(fTBN * normal);


	//gAlbedo = vec4(albedo, roughness);
	//gNormal = vec4(normal, metallic);
	//gNormal = vec4(normal, 1.0);

}
