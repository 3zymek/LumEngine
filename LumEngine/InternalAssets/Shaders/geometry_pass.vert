#version 460
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_UV;
/*
layout( set = 0, binding = 0, std140 ) uniform CameraUniforms {
	mat4 m_View;
	mat4 m_Projection;
	mat4 m_InvertedProjection;
	vec4 m_Position;
} u_CameraData;
*/

layout( push_constant, std140 ) uniform PushConstants {
	mat4 m_Matrix;
} pc_Model;

layout(location = 0) out vec3 f_Position;
layout(location = 1) out vec2 f_UV;

//out mat3 fTBN;

void main( ) {

	gl_Position = vec4(in_Position, 1.0);

	f_Position = in_Position;
	f_UV = in_UV;

	/*
	gl_Position = 
		u_CameraData.m_Projection *
		u_CameraData.m_View *
		pc_Model.m_Matrix *
		vec4( in_Pos, 1.0 );
	
	//mat3 normalMatrix = transpose(inverse(mat3(uModelMatrix)));
	f_UV = in_UV;
	f_Pos = (pc_Model.m_Matrix * vec4(in_Pos, 1.0)).xyz;
	
	//vec3 T = normalize(normalMatrix * aTangent);
	//vec3 B = normalize(normalMatrix * aBitangent);
	//vec3 N = normalize(normalMatrix * aNormal);
	//fTBN = mat3(T, B, N);
	*/
}