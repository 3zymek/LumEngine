//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Loads assets (textures, sounds, shaders, models)
// 
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/types.hpp"

namespace lum {

	struct Vertex;

	namespace detail {

		namespace fs = std::filesystem;

	}

	struct TextureData {

		int32 mWidth = 0;
		int32 mHeight = 0;
		int32 mColorChannels = 0;
		std::vector<uint8> mPixels;

	};

	struct ModelData {

		std::vector<Vertex> mVertices;
		std::vector<uint32> mIndices;

	};
	
	class AssetService {

		using Path = detail::fs::path;

	public:

		LUM_FORCEINLINE
		static void SetProjectRoot ( Path path ) { 
			gProjectRoot = path; 
			LUM_LOG_INFO("Project root path has been set to %s", path.string().c_str());
		}

		LUM_NODISCARD
		static TextureData LoadTexture ( ccharptr filepath, bool& success );
		
		LUM_NODISCARD
		static ModelData LoadMesh ( ccharptr filepath, bool& success );

		LUM_NODISCARD
		static String LoadAudio ( StringView filepath, bool& success );

		LUM_NODISCARD
		static String LoadInternalShader ( StringView filepath, bool& success );

		LUM_NODISCARD
		static String LoadExternalShader ( StringView filepath, bool& success );

	private:

		static inline Path gProjectRoot			= "";
		static inline Path gInternalShaderPath	= detail::fs::current_path().parent_path() / "LumEngine" / "engine" / "include" / "modules" / "rhi" / "shaders";
		static inline Path gShaderDefine		= detail::fs::current_path().parent_path() / "LumEngine" / "engine" / "include" / "core" / "shaders_define.h";

		AssetService ( const AssetService& )	= delete;
		AssetService ( AssetService&& )			= delete;
		AssetService ( )						= default;

	};
}