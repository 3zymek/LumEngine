#version 450 core
layout( location = LUM_LAYOUT_POSITION	) in vec3 aPos;
layout( location = LUM_LAYOUT_NORMAL	) in vec3 aNormal;
layout( location = LUM_LAYOUT_UV		) in vec2 aUV;
layout( location = LUM_LAYOUT_TANGENT	) in vec3 aTangent;
layout( location = LUM_LAYOUT_BITANGENT	) in vec3 aBitangent;
layout( std140, binding = LUM_UBO_CAMERA_BINDING ) uniform CameraUniforms {
	
	mat4 uCameraView;
	mat4 uCameraProj;
	mat4 uCameraInvProj;
	vec4 uCameraPos;

};
layout( std140, binding = LUM_UBO_MODEL_BINDING ) uniform ModelUniforms {

	mat4 uModelMatrix;
	
};

out vec2 fUV;
out vec3 fPos;

out mat3 fTBN;

void main( ) {
	
	gl_Position = 
		uCameraProj *
		uCameraView *
		uModelMatrix *
		vec4(aPos, 1.0);
	
	mat3 normalMatrix = transpose(inverse(mat3(uModelMatrix)));
	fUV = aUV;
	fPos = (uModelMatrix * vec4(aPos, 1.0)).xyz;
	
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 B = normalize(normalMatrix * aBitangent);
	vec3 N = normalize(normalMatrix * aNormal);
	fTBN = mat3(T, B, N);

}