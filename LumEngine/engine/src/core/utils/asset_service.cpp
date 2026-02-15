//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Loads assets (textures, sounds, shaders, models)
// 
//=============================================================================//
#pragma once

//====== Backend ======
#include <stb_image.h>			// Loading textures
#include <assimp/Importer.hpp>	// Loading Models
#include <assimp/scene.h>		// Loading Models
#include <assimp/postprocess.h> // Loading Models
//=====================

#include "core/utils/asset_service.hpp"
#include "core/utils/lum_assert.hpp"
#include "rhi/rhi_common.hpp"


namespace lum {
		
		TextureData AssetService::LoadTexture ( ccharptr filepath, bool& success ) {

			String file = (gProjectRoot / String(filepath)).lexically_normal().string();

			if (!detail::fs::exists(file)) {
				success = false;
				return {};
			}

			TextureData texture;
			int32 format;
			
			ucharptr data = stbi_load(file.c_str(), &texture.mWidth, &texture.mHeight, &format, 4);
			texture.mColorChannels = 4;

			if (!data) {
				success = false;
				return {};
			}

			usize size = texture.mWidth * texture.mHeight * texture.mColorChannels;
			texture.mPixels.resize(size);

			LUM_ASSERT(size > 0 && data != nullptr, "Texture source data is null");

			std::memcpy(texture.mPixels.data(), data, size);

			stbi_image_free(data);
			success = true;

			return texture;
		}

		
		ModelData AssetService::LoadMesh ( ccharptr filepath, bool& success ) {

			String file = (gProjectRoot / filepath).lexically_normal().string();
			if (!detail::fs::exists(file)) {
				success = false;
				return {};
			}

			Assimp::Importer importer;
			uint32 flags = 0;
			flags |= aiProcess_FlipUVs;
			flags |= aiProcess_Triangulate;
			flags |= aiProcess_CalcTangentSpace;
			const aiScene* scene = importer.ReadFile(file.c_str(), flags);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				success = false;
				return {};
			}
		   
			ModelData finalData;

			uint32 elementOffset = 0;

			for (uint32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {

				aiMesh* mesh = scene->mMeshes[meshIndex];

				for (uint32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
					Vertex vert;

					vert.position = {
						mesh->mVertices[vertexIndex].x,
						mesh->mVertices[vertexIndex].y,
						mesh->mVertices[vertexIndex].z
					};

					if (mesh->HasNormals()) {
						vert.normal = {
							mesh->mNormals[vertexIndex].x,
							mesh->mNormals[vertexIndex].y,
							mesh->mNormals[vertexIndex].z
						};
					}
					else vert.normal = { 0.0f, 0.0f, 0.0f };

					if (mesh->mTextureCoords[0]) {
						vert.uv = {
							mesh->mTextureCoords[0][vertexIndex].x,
							mesh->mTextureCoords[0][vertexIndex].y
						};

					}
					else vert.uv = { 0.0f, 0.0f };
					
					finalData.mVertices.push_back(vert);
				  
				}

				for (uint32 faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
					aiFace face = mesh->mFaces[faceIndex];

					for (uint32 elementIndex = 0; elementIndex < face.mNumIndices; ++elementIndex) {
						finalData.mIndices.push_back(face.mIndices[elementIndex] + elementOffset);
					}

					
				}
				elementOffset += mesh->mNumVertices;

		   }

			success = true;
			return finalData;
		}
	   

		String AssetService::LoadAudio ( StringView filepath, bool& success ) {

			String file = (gProjectRoot / filepath).lexically_normal().string();
			 
			if (!detail::fs::exists(file)) {
				success = false;
				return {};
			}

			success = true;
			return file;
		}


		String AssetService::LoadInternalShader ( StringView filepath, bool& success ) {

			String file = (gInternalShaderPath / filepath).lexically_normal().string();

			if (!detail::fs::exists(file)) {
				success = false;
				return "";
			}

			std::ifstream loadedFile(file);
			std::ifstream defines(gShaderDefine);
			if (!loadedFile.is_open() || !defines.is_open()) {
				success = false;
				return "";
			}

			String version;
			std::getline(loadedFile, version);

			std::stringstream ss;
			ss << version << '\n';
			ss << defines.rdbuf() << '\n';
			ss << loadedFile.rdbuf();

			success = true;
			return ss.str();
		}


		String AssetService::LoadExternalShader ( StringView filepath, bool& success ) {

			String file = (gProjectRoot / filepath).lexically_normal().string();

			if (!detail::fs::exists(file)) {
				success = false;
				return "";
			}

			std::ifstream loadedFile(file);
			std::ifstream defines(gShaderDefine);
			if (!loadedFile.is_open() || !defines.is_open()) {
				success = false;
				return "";
			}

			String version;
			std::getline(loadedFile, version);

			std::stringstream ss;
			ss << version << '\n';
			ss << defines.rdbuf() << '\n';
			ss << loadedFile.rdbuf();

			success = true;
			return ss.str();
		}

}