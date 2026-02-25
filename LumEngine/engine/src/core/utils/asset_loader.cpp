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

#include "core/utils/asset_loader.hpp"
#include "core/utils/lum_assert.hpp"
#include "rhi/rhi_common.hpp"


namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	std::optional<FTextureData> AssetLoader::LoadTexture( ERootID root, StringView filepath ) {

		String path = get_full_path(root, filepath);

		if (!detail::fs::exists(path)) {
			set_error_msg("File doesn't exist");
			return std::nullopt;
		}

		FTextureData texture;
		int32 format;

		ucharptr data = stbi_load(path.c_str(), &texture.mWidth, &texture.mHeight, &format, 4);
		texture.mColorChannels = 4;

		if (!data) {
			set_error_msg(stbi_failure_reason());
			return std::nullopt;
		}

		usize size = texture.mWidth * texture.mHeight * texture.mColorChannels;
		texture.mPixels.resize(size);

		LUM_ASSERT(size > 0 && data != nullptr, "Texture source data is null");

		std::memcpy(texture.mPixels.data(), data, size);

		stbi_image_free(data);

		return texture;
	}


	std::optional<FMeshData> AssetLoader::LoadMesh( ERootID root, StringView filepath ) {

		String path = get_full_path(root, filepath);

		if (!detail::fs::exists(path)) {
			set_error_msg("File doesn't exist");
			return std::nullopt;
		}

		Assimp::Importer importer;
		uint32 flags = 0;
		flags |= aiProcess_FlipUVs;
		flags |= aiProcess_Triangulate;
		flags |= aiProcess_CalcTangentSpace;
		const aiScene* scene = importer.ReadFile(path.c_str(), flags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			set_error_msg(importer.GetErrorString());
			return std::nullopt;
		}

		FMeshData finalData;

		uint32 elementOffset = 0;

		for (uint32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {

			aiMesh* mesh = scene->mMeshes[meshIndex];

			for (uint32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
				rhi::FVertex vert;

				vert.mPosition = {
					mesh->mVertices[vertexIndex].x,
					mesh->mVertices[vertexIndex].y,
					mesh->mVertices[vertexIndex].z
				};

				if (mesh->HasNormals()) {
					vert.mNormal = {
						mesh->mNormals[vertexIndex].x,
						mesh->mNormals[vertexIndex].y,
						mesh->mNormals[vertexIndex].z
					};
				}
				else vert.mNormal = { 0.0f, 0.0f, 0.0f };

				if (mesh->mTextureCoords[0]) {
					vert.mUv = {
						mesh->mTextureCoords[0][vertexIndex].x,
						mesh->mTextureCoords[0][vertexIndex].y
					};

				}
				else vert.mUv = { 0.0f, 0.0f };

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

		return finalData;
	}


	std::optional<String> AssetLoader::LoadAudio( ERootID root, StringView filepath ) {

		String path = get_full_path(root, filepath);

		if (!detail::fs::exists(path)) {
			set_error_msg("File doesn't exist");
			return std::nullopt;
		}

		return path;
	}


	std::optional<String> AssetLoader::LoadShader( ERootID root, StringView filepath ) {

		String path = get_full_path(root, filepath);

		std::ifstream loadedFile(path);
		if (!loadedFile.is_open()) {
			set_error_msg(strerror(errno));
			return std::nullopt;
		}

		std::ifstream defines(sShaderDefine);
		if (!defines.is_open()) {
			set_error_msg(strerror(errno));
			return std::nullopt;
		}

		String version;
		std::getline(loadedFile, version);

		std::stringstream ss;
		ss << version << '\n';
		ss << defines.rdbuf() << '\n';
		ss << loadedFile.rdbuf();

		return ss.str();
	}

	bool AssetLoader::WriteFile( ERootID root, StringView filepath, const String& content ) {

		String path = get_full_path(root, filepath);

		std::ofstream file(path);
		if (!file.is_open()) {
			set_error_msg(strerror(errno));
			return false;
		}
		file << content;
		file.close();

		return true;
	}
	std::optional<String> AssetLoader::ReadFile(ERootID root, StringView filepath) {
		
		String path = get_full_path(root, filepath);

		std::ifstream file(path);
		if (!file.is_open()) {
			set_error_msg(strerror(errno));
			return std::nullopt;
		}
		
		std::ostringstream ss;
		ss << file.rdbuf();

		String content = ss.str();
		
		file.close();

		return content;
	}

}