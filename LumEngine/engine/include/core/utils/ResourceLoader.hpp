//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Loads assets (textures, sounds, shaders, models)
// 
//=============================================================================//
#pragma once

#include "Core/Utils/Result.hpp"
#include "Core/CoreCommon.hpp"
#include "Core/Limits.hpp"
#include "Platform/FileSystem.hpp"

namespace lum {

	struct Vertex;
	namespace detail {

		namespace fs = std::filesystem;

	} // namespace lum::detail

	/* @brief Identifies the root directory used for asset path resolution. */
	enum class LUM_API ResourceRoot : byte {
		Internal, // Engine internal assets directory.
		External  // Project root directory set by the user.
	};

	/* @brief Raw texture data loaded from disk. */
	struct LUM_API ImageData {

		/* @brief Texture width in pixels. */
		int32 mWidth = 0;

		/* @brief Texture height in pixels. */
		int32 mHeight = 0;

		/* @brief Number of color channels (e.g. 3 = RGB, 4 = RGBA). */
		int32 mChannels = 0;

		/* @brief Raw pixel data. */
		std::vector<uint8> mPixels;

		/* @brief Raw float pixels used in HDRI. */
		std::vector<float32> mFloatPixels;

		bool mIsHdr = false;

	};

	/* @brief Raw mesh data loaded from disk. */
	struct LUM_API MeshGeometry {

		/* @brief List of vertices. */
		std::vector<Vertex> mVertices;

		/* @brief List of indices. */
		std::vector<uint32> mIndices;

	};

	enum class LUM_API ImageFormat : uint8 {
		Native	= 0,
		R		= 1,
		RG		= 2,
		RGB		= 3,
		RGBA	= 4
	};


	/* @brief Static utility class for loading engine assets from disk.
	*
	* Resolves asset paths relative to either the internal engine root
	* or the user-defined project root. All load methods return std::optional
	* and set an error message on failure, retrievable via GetErrorMessage().
	*
	* Non-constructible and non-copyable — use static methods only.
	*/
	class LUM_API ResourceLoader {
	public:

		/* @brief Sets the project root directory for external asset resolution.
		* @param path Absolute path to the project root.
		*/
		static void SetProjectRoot( Path path ) { sProjectRoot = path; }

		/* @brief Returns the current project root path as a string. */
		LUM_NODISCARD static String GetProjectRoot( ) { return sProjectRoot.ToString( ); }

		/* @brief Loads a texture from disk.
		* @param root           Root directory identifier.
		* @param filepath       Path relative to the selected root.
		* @param expectedFormat Number of channels to force (1-4).
		* @return TextureData on success, or empty on failure.
		*/
		LUM_NODISCARD static Result<ImageData> LoadImageFromFile( ResourceRoot root, StringView filepath, ImageFormat expectedFormat = ImageFormat::Native );

		/* @brief Loads a mesh from disk.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return Populated MeshData or empty on failure.
		*/
		LUM_NODISCARD static Result<MeshGeometry> LoadMeshFromFile( ResourceRoot root, StringView filepath );

		/* @brief Loads a shader source file from disk.
		* Prepends the engine shader define header automatically.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return Shader source as String or empty on failure.
		*/
		LUM_NODISCARD static Result<String> BuildShaderSource( ResourceRoot root, StringView filepath );

		/* @brief Resolves an absolute path from a root directory and a relative filepath.
		* @param root     Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return Resolved absolute path, or empty if the file does not exist.
		*/
		static Path ResolveResourcePath( ResourceRoot root, StringView filepath );

	private:

		/* @brief Absolute path to the project root (external assets). */
		static inline Path sProjectRoot{ "" };

		/* @brief Absolute path to the engine internal assets directory. */
		static inline Path sInternalAssetsRoot = FileSystem::CurrentPath().ParentPath( ) / "LumEngine" / "InternalAssets";

		/* @brief Path to the shared shader define header prepended to all shaders. */
		static inline Path sShaderDefineFile = FileSystem::CurrentPath( ).ParentPath( ) / "LumEngine" / "Engine" / "Include" / "Modules" / "Render" / "ShadersDefine.h";

		ResourceLoader( const ResourceLoader& ) = delete;
		ResourceLoader( ResourceLoader&& ) = delete;
		ResourceLoader( ) = default;

	};
} // namespace lum