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

	namespace rhi { struct FVertex; }
	namespace detail {

		namespace fs = std::filesystem;
	
	} // namespace lum::detail

	/* @brief Identifies the root directory used for asset path resolution. */
	enum class ERootID : byte {
		Internal, // Engine internal assets directory.
		External  // Project root directory set by the user.
	};

	/* @brief Raw texture data loaded from disk. */
	struct FTextureData {
		/* @brief Texture width in pixels. */
		int32 mWidth = 0;
		/* @brief Texture height in pixels. */
		int32 mHeight = 0;
		/* @brief Number of color channels (e.g. 3 = RGB, 4 = RGBA). */
		int32 mChannels = 0;
		/* @brief Raw pixel data. */
		std::vector<uint8> mPixels;
	};

	/* @brief Raw mesh data loaded from disk. */
	struct FMeshData {
		/* @brief List of vertices. */
		std::vector<rhi::FVertex> mVertices;
		/* @brief List of indices. */
		std::vector<uint32> mIndices;
	};


	/* @brief Static utility class for loading engine assets from disk.
	*
	* Resolves asset paths relative to either the internal engine root
	* or the user-defined project root. All load methods return std::optional
	* and set an error message on failure, retrievable via GetErrorMessage().
	*
	* @note Non-constructible and non-copyable — use static methods only.
	*/
	class AssetLoader {

		using Path = detail::fs::path;

	public:

		/* @brief Sets the project root directory for external asset resolution.
		* @param path Absolute path to the project root.
		*/
		LUM_FORCEINLINE
		static void SetProjectRoot( Path path ) {
			sProjectRoot = path;
			LUM_LOG_INFO("Project root path has been set to %s", path.string().c_str());
		}

		/* @brief Returns the current project root path as a string. */
		LUM_NODISCARD
		static String GetProjectRoot( ) {
			return sProjectRoot.string();
		}
		
		/* @brief Loads a texture from disk.
		* @param root           Root directory identifier.
		* @param filepath       Path relative to the selected root.
		* @param expectedFormat Number of channels to force (1-4). 0 = use native format from file.
		* @return FTextureData on success, or empty on failure.
		*/
		LUM_NODISCARD
		static std::optional<FTextureData> LoadTexture( ERootID root, StringView filepath, uint8 expectedFormat = 0 );

		/* @brief Loads a mesh from disk.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return Populated FMeshData or empty on failure.
		*/
		LUM_NODISCARD
		static std::optional<FMeshData> LoadMesh( ERootID root, StringView filepath );

		/* @brief Loads an audio file from disk.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return File contents as String or empty on failure.
		*/
		LUM_NODISCARD
		static std::optional<String> LoadAudio( ERootID root, StringView filepath );

		/* @brief Loads a shader source file from disk.
		* Prepends the engine shader define header automatically.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return Shader source as String or empty on failure.
		*/
		LUM_NODISCARD
		static std::optional<String> LoadShader( ERootID root, StringView filepath );

		/* @brief Writes text content to a file at the given path.
		* Creates the file if it does not exist, overwrites if it does.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @param content Text content to write.
		* @return If operation went succesfully.
		*/
		static bool WriteFile( ERootID root, StringView filepath, const String& content );

		/* @brief Reads raw text content from a file.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return File contents as String or empty on failure.
		*/
		LUM_NODISCARD
		static std::optional<String> ReadFile( ERootID root, StringView filepath );

		/* @brief Returns the last error message set by a failed load operation. */
		static ccharptr GetErrorMessage( ) { return sLastErrorMessage; }

	private:

		/* @brief Absolute path to the project root (external assets). */
		static inline Path sProjectRoot = "";

		/* @brief Absolute path to the engine internal assets directory. */
		static inline Path sInternalRoot = detail::fs::current_path().parent_path() / "LumEngine" / "internal_assets";

		/* @brief Path to the shared shader define header prepended to all shaders. */
		static inline Path sShaderDefine = detail::fs::current_path().parent_path() / "LumEngine" / "engine" / "include" / "core" / "shaders_define.h";

		/* @brief Buffer storing the last asset load error message. */
		static inline char sLastErrorMessage[limits::gMaxErrorAssetLoadLength]{};

		/* @brief Sets the error message from a string literal. */
		template<usize tL>
		static void set_error_msg( const char(&msg)[tL] ) {
			usize length = (tL < limits::gMaxErrorAssetLoadLength) ? tL : limits::gMaxErrorAssetLoadLength;
			std::strncpy(sLastErrorMessage, msg, length);
		}

		/* @brief Sets the error message from a runtime string. */
		static void set_error_msg( String msg ) {
			std::strncpy(sLastErrorMessage, msg.data(), sizeof(sLastErrorMessage));
		}

		static String get_full_path( ERootID root, StringView filepath ) {
			if (root == ERootID::External)
				return (sProjectRoot / filepath).lexically_normal().string();
			else if (root == ERootID::Internal)
				return (sInternalRoot / filepath).lexically_normal().string();
			return "";
		}

		AssetLoader( const AssetLoader& ) = delete;
		AssetLoader( AssetLoader&& ) = delete;
		AssetLoader( ) = default;

	};
} // namespace lum