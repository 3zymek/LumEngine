//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Loads assets (textures, sounds, shaders, models)
// 
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/types.hpp"
#include "core/limits.hpp"

namespace lum {

	struct Vertex;

	namespace detail {

		namespace fs = std::filesystem;

	}

	enum class ERootID : uint8 {
		Internal,
		External
	};

	struct FTextureData {

		int32 mWidth = 0;
		int32 mHeight = 0;
		int32 mColorChannels = 0;
		std::vector<uint8> mPixels;

	};

	struct FModelData {

		std::vector<Vertex> mVertices;
		std::vector<uint32> mIndices;

	};
	
	class AssetLoader {

		using Path = detail::fs::path;

	public:

		LUM_FORCEINLINE
		static void SetProjectRoot ( Path path ) { 
			sProjectRoot = path; 
			LUM_LOG_INFO("Project root path has been set to %s", path.string().c_str());
		}

		LUM_NODISCARD
		static std::optional<FTextureData> LoadTexture ( ERootID root, ccharptr filepath );
		
		LUM_NODISCARD
		static std::optional<FModelData> LoadMesh ( ERootID root, ccharptr filepath );

		LUM_NODISCARD
		static std::optional<String> LoadAudio ( ERootID root, StringView filepath );

		LUM_NODISCARD
		static std::optional<String> LoadShader ( ERootID root, StringView filepath );

		static ccharptr GetErrorMessage() { return sLastErrorMessage; }

	private:

		static inline Path sProjectRoot		= "";
		static inline Path sInternalRoot	= detail::fs::current_path().parent_path() / "LumEngine" / "internal_assets";
		static inline Path sShaderDefine	= detail::fs::current_path().parent_path() / "LumEngine" / "engine" / "include" / "core" / "shaders_define.h";
	   
		static inline char sLastErrorMessage[limits::gMaxErrorAssetLoadLength] {};

		template<usize L>
		static void set_error_msg ( const char (&msg)[L] ) {

			usize length = std::clamp(L, 0u, limits::gMaxErrorAssetLoadLength);
			std::strncpy(sLastErrorMessage, msg, length);

		}

		static void set_error_msg ( ccharptr msg ) {

			std::strncpy(sLastErrorMessage, msg, sizeof(sLastErrorMessage));

		}

		AssetLoader ( const AssetLoader& )	= delete;
		AssetLoader ( AssetLoader&& )		= delete;
		AssetLoader ( )						= default;

	};
}