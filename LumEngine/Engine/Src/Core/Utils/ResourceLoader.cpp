//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Loads assets (textures, sounds, shaders, models)
// 
//=============================================================================//
#pragma once

#include "Core/Utils/ResourceLoader.hpp"
#include "Rhi/RhiCommon.hpp"

//====== Backend ======
#include <stb_image.h>			// Loading textures
#include <Assimp/Importer.hpp>	// Loading Models
#include <Assimp/Scene.h>		// Loading Models
#include <Assimp/Postprocess.h> // Loading Models
//=====================

namespace lum {

	//=======================================================//
	// Public
	//=======================================================//

	Result<ImageData> ResourceLoader::LoadImageFromFile( ResourceRoot root, const Path& filepath, ImageFormat expectedFormat ) {

		Path path = ResolveResourcePath( root, filepath );
		String strPath = path.ToString( );
		uint8 formatCast = ToUnderlyingEnum( expectedFormat );

		if (!FileSystem::Exists( path )) {
			return Result<ImageData>::Failure( "File doesn't exist" );
		}

		ImageData texture;
		texture.m_IsHdr = path.EndsWith( ".hdr" );

		int32 format{};

		if (texture.m_IsHdr) {

			float32* data = stbi_loadf( strPath.c_str(), &texture.m_Width, &texture.m_Height, &format, formatCast );
			if (!data) {
				return Result<ImageData>::Failure( stbi_failure_reason( ) );
			}
			texture.m_Channels = (formatCast != 0) ? formatCast : format;
			texture.m_FloatPixels.assign( data, data + texture.m_Width * texture.m_Height * texture.m_Channels );
			stbi_image_free( data );

		}
		else {

			unsigned char* data = stbi_load( strPath.c_str( ), &texture.m_Width, &texture.m_Height, &format, formatCast );
			if (!data) {
				return Result<ImageData>::Failure( stbi_failure_reason( ) );
			}
			texture.m_Channels = (formatCast != 0) ? formatCast : format;
			texture.m_Pixels.assign( data, data + texture.m_Width * texture.m_Height * texture.m_Channels );
			stbi_image_free( data );

		}

		return texture;
	}

	Result<MeshGeometry> ResourceLoader::LoadMeshFromFile( ResourceRoot root, const Path& filepath ) {

		Path path = ResolveResourcePath( root, filepath );

		if (!FileSystem::Exists( path )) {
			return Result<MeshGeometry>::Failure( "File doesn't exist" );
		}

		Assimp::Importer importer;
		uint32 flags = 0;
		flags |= aiProcess_FlipUVs;
		flags |= aiProcess_Triangulate;
		flags |= aiProcess_CalcTangentSpace;
		const aiScene* scene = importer.ReadFile( path.ToString( ), flags );

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			return Result<MeshGeometry>::Failure( importer.GetErrorString( ) );
		}

		MeshGeometry finalData;

		uint32 elementOffset = 0;

		for (uint32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {

			aiMesh* mesh = scene->mMeshes[ meshIndex ];

			for (uint32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
				Vertex vert;

				vert.m_Position = {
					mesh->mVertices[ vertexIndex ].x,
					mesh->mVertices[ vertexIndex ].y,
					mesh->mVertices[ vertexIndex ].z
				};

				if (mesh->HasNormals( )) {
					vert.m_Normal = {
						mesh->mNormals[ vertexIndex ].x,
						mesh->mNormals[ vertexIndex ].y,
						mesh->mNormals[ vertexIndex ].z
					};
				}
				else vert.m_Normal = { 0.0f, 0.0f, 0.0f };

				if (mesh->HasTangentsAndBitangents( )) {
					vert.m_Tangent = {
						mesh->mTangents[ vertexIndex ].x,
						mesh->mTangents[ vertexIndex ].y,
						mesh->mTangents[ vertexIndex ].z
					};
					vert.m_Bitangent = {
						mesh->mBitangents[ vertexIndex ].x,
						mesh->mBitangents[ vertexIndex ].y,
						mesh->mBitangents[ vertexIndex ].z
					};
				}
				else {
					vert.m_Tangent = { 0.0f, 0.0f, 0.0f };
					vert.m_Bitangent = { 0.0f, 0.0f, 0.0f };
				}

				if (mesh->mTextureCoords[ 0 ]) {
					vert.m_Uv = {
						mesh->mTextureCoords[ 0 ][ vertexIndex ].x,
						mesh->mTextureCoords[ 0 ][ vertexIndex ].y
					};

				}
				else vert.m_Uv = { 0.0f, 0.0f };

				finalData.m_Vertices.push_back( vert );

			}

			for (uint32 faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
				aiFace face = mesh->mFaces[ faceIndex ];

				for (uint32 elementIndex = 0; elementIndex < face.mNumIndices; ++elementIndex) {
					finalData.m_Indices.push_back( face.mIndices[ elementIndex ] + elementOffset );
				}


			}
			elementOffset += mesh->mNumVertices;

		}

		return finalData;
	}


	Result<String> ResourceLoader::BuildShaderSource( ResourceRoot root, const Path& filepath ) {

		Path path = ResolveResourcePath( root, filepath );

		auto shaderSource = FileSystem::ReadAllText( path );
		if (!shaderSource) {
			return shaderSource;
		}

		auto defines = FileSystem::ReadAllText( s_ShaderDefineFile );
		if (!defines) {
			return defines;
		}

		String version{};
		String clippedSource{};
		usize versionIdx = shaderSource->find( "#version" );
		if (versionIdx != String::npos) {

			usize end = shaderSource->find( '\n', versionIdx );
			version = shaderSource->substr( versionIdx, end - versionIdx );
			clippedSource = shaderSource->substr( end );
		
		}

		std::stringstream ss;
		ss << version << '\n';
		ss << defines.ValueRef() << '\n';
		ss << clippedSource;

		return ss.str( );
	}

	Path ResourceLoader::ResolveResourcePath( ResourceRoot root, const Path& filepath ) {
		if (root == ResourceRoot::External)
			return (s_ProjectRoot / filepath).LexicallyNormal( );
		else if (root == ResourceRoot::Internal)
			return (s_InternalAssetsRoot / filepath).LexicallyNormal( );
		return Path( );
	}

}