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

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	std::optional<ImageData> ResourceLoader::LoadImageFromFile( ResourceRoot root, StringView filepath, uint8 expectedFormat ) {

		String path = get_full_path( root, filepath );

		if (!detail::fs::exists( path )) {
			set_error_msg( "File doesn't exist" );
			return std::nullopt;
		}

		ImageData texture;
		texture.mIsHdr = path.ends_with( ".hdr" );

		int32 format{};

		if (texture.mIsHdr) {

			float32* data = stbi_loadf( path.c_str( ), &texture.mWidth, &texture.mHeight, &format, expectedFormat );
			if (!data) {
				set_error_msg( stbi_failure_reason( ) );
				return std::nullopt;
			}
			texture.mChannels = (expectedFormat != 0) ? expectedFormat : format;
			texture.mFloatPixels.assign( data, data + texture.mWidth * texture.mHeight * texture.mChannels );
			stbi_image_free( data );

		}
		else {

			ucharptr data = stbi_load( path.c_str( ), &texture.mWidth, &texture.mHeight, &format, expectedFormat );
			if (!data) {
				set_error_msg( stbi_failure_reason( ) );
				return std::nullopt;
			}
			texture.mChannels = (expectedFormat != 0) ? expectedFormat : format;
			texture.mPixels.assign( data, data + texture.mWidth * texture.mHeight * texture.mChannels );
			stbi_image_free( data );

		}

		return texture;
	}

	std::optional<MeshGeometry> ResourceLoader::LoadMeshFromFile( ResourceRoot root, StringView filepath ) {

		String path = get_full_path( root, filepath );

		if (!detail::fs::exists( path )) {
			set_error_msg( "File doesn't exist" );
			return std::nullopt;
		}

		Assimp::Importer importer;
		uint32 flags = 0;
		flags |= aiProcess_FlipUVs;
		flags |= aiProcess_Triangulate;
		flags |= aiProcess_CalcTangentSpace;
		const aiScene* scene = importer.ReadFile( path.c_str( ), flags );

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			set_error_msg( importer.GetErrorString( ) );
			return std::nullopt;
		}

		MeshGeometry finalData;

		uint32 elementOffset = 0;

		for (uint32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {

			aiMesh* mesh = scene->mMeshes[ meshIndex ];

			for (uint32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
				Vertex vert;

				vert.mPosition = {
					mesh->mVertices[ vertexIndex ].x,
					mesh->mVertices[ vertexIndex ].y,
					mesh->mVertices[ vertexIndex ].z
				};

				if (mesh->HasNormals( )) {
					vert.mNormal = {
						mesh->mNormals[ vertexIndex ].x,
						mesh->mNormals[ vertexIndex ].y,
						mesh->mNormals[ vertexIndex ].z
					};
				}
				else vert.mNormal = { 0.0f, 0.0f, 0.0f };

				if (mesh->HasTangentsAndBitangents( )) {
					vert.mTangent = {
						mesh->mTangents[ vertexIndex ].x,
						mesh->mTangents[ vertexIndex ].y,
						mesh->mTangents[ vertexIndex ].z
					};
					vert.mBitangent = {
						mesh->mBitangents[ vertexIndex ].x,
						mesh->mBitangents[ vertexIndex ].y,
						mesh->mBitangents[ vertexIndex ].z
					};
				}
				else {
					vert.mTangent = { 0.0f, 0.0f, 0.0f };
					vert.mBitangent = { 0.0f, 0.0f, 0.0f };
				}

				if (mesh->mTextureCoords[ 0 ]) {
					vert.mUv = {
						mesh->mTextureCoords[ 0 ][ vertexIndex ].x,
						mesh->mTextureCoords[ 0 ][ vertexIndex ].y
					};

				}
				else vert.mUv = { 0.0f, 0.0f };

				finalData.mVertices.push_back( vert );

			}

			for (uint32 faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
				aiFace face = mesh->mFaces[ faceIndex ];

				for (uint32 elementIndex = 0; elementIndex < face.mNumIndices; ++elementIndex) {
					finalData.mIndices.push_back( face.mIndices[ elementIndex ] + elementOffset );
				}


			}
			elementOffset += mesh->mNumVertices;

		}

		return finalData;
	}


	String ResourceLoader::ResolvePath( ResourceRoot root, StringView filepath ) {

		String path = get_full_path( root, filepath );

		if (!detail::fs::exists( path )) {
			set_error_msg( "File doesn't exist" );
			return "";
		}

		return path;
	}


	String ResourceLoader::BuildShaderSource( ResourceRoot root, StringView filepath ) {

		String path = get_full_path( root, filepath );

		std::ifstream loadedFile( path );
		if (!loadedFile.is_open( )) {
			set_error_msg( strerror( errno ) );
			return "";
		}

		std::ifstream defines( sShaderDefineFile );
		if (!defines.is_open( )) {
			set_error_msg( strerror( errno ) );
			return "";
		}

		String version;
		std::getline( loadedFile, version );

		std::stringstream ss;
		ss << version << '\n';
		ss << defines.rdbuf( ) << '\n';
		ss << loadedFile.rdbuf( );

		return ss.str( );
	}

	bool ResourceLoader::WriteTextFile( ResourceRoot root, StringView filepath, const String& content ) {

		String path = get_full_path( root, filepath );

		std::ofstream file( path );
		if (!file.is_open( )) {
			set_error_msg( strerror( errno ) );
			return false;
		}
		file << content;
		file.close( );

		return true;
	}
	String ResourceLoader::ReadTextFile( ResourceRoot root, StringView filepath ) {

		String path = get_full_path( root, filepath );

		std::ifstream file( path );
		if (!file.is_open( )) {
			set_error_msg( strerror( errno ) );
			return "";
		}

		std::ostringstream ss;
		ss << file.rdbuf( );

		String content = ss.str( );

		file.close( );

		return content;
	}

}