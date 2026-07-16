//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Loads assets (textures, sounds, shaders, models)
// 
//=============================================================================//
#pragma once
#include "Core/CoreCommon.hpp"
#include "Core/Limits.hpp"

namespace lum {

	struct Vertex;
	namespace detail {

		namespace fs = std::filesystem;

	} // namespace lum::detail

	/* @brief Identifies the root directory used for asset path resolution. */
	enum class ResourceRoot : byte {
		Internal, // Engine internal assets directory.
		External  // Project root directory set by the user.
	};

	/* @brief Raw texture data loaded from disk. */
	struct ImageData {

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
	struct MeshGeometry {

		/* @brief List of vertices. */
		std::vector<Vertex> mVertices;

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
	class ResourceLoader {

		using FilePath = detail::fs::path;

	public:

		/* @brief Sets the project root directory for external asset resolution.
		* @param path Absolute path to the project root.
		*/
		static void SetProjectRoot( FilePath path ) { sProjectRoot = path; }

		/* @brief Returns the current project root path as a string. */
		LUM_NODISCARD
			static String GetProjectRoot( ) { return sProjectRoot.string( ); }

		/* @brief Loads a texture from disk.
		* @param root           Root directory identifier.
		* @param filepath       Path relative to the selected root.
		* @param expectedFormat Number of channels to force (1-4). 0 = use native format from file.
		* @return TextureData on success, or empty on failure.
		*/
		LUM_NODISCARD
			static std::optional<ImageData> LoadImageFromFile( ResourceRoot root, StringView filepath, uint8 expectedFormat = 0 );

		/* @brief Loads a mesh from disk.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return Populated MeshData or empty on failure.
		*/
		LUM_NODISCARD
			static std::optional<MeshGeometry> LoadMeshFromFile( ResourceRoot root, StringView filepath );

		/* @brief Resolves an absolute path from a root directory and a relative filepath.
		* @param root     Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return Resolved absolute path, or empty if the file does not exist.
		*/
		LUM_NODISCARD
			static String ResolvePath( ResourceRoot root, StringView filepath );

		/* @brief Loads a shader source file from disk.
		* Prepends the engine shader define header automatically.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return Shader source as String or empty on failure.
		*/
		LUM_NODISCARD
			static String BuildShaderSource( ResourceRoot root, StringView filepath );

		/* @brief Writes text content to a file at the given path.
		* Creates the file if it does not exist, overwrites if it does.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @param content Text content to write.
		* @return True if operation completed successfully.
		*/
		static bool WriteTextFile( ResourceRoot root, StringView filepath, const String& content );

		/* @brief Reads raw text content from a file.
		* @param root Root directory identifier.
		* @param filepath Path relative to the selected root.
		* @return File contents as String or empty String on failure.
		*/
		LUM_NODISCARD
			static String ReadTextFile( ResourceRoot root, StringView filepath );

		/* @brief Returns the last error message set by a failed load operation. */
		static ccharptr GetErrorMessage( ) { return sLastErrorMessage; }

	private:

		/* @brief Absolute path to the project root (external assets). */
		static inline FilePath sProjectRoot = "";

		/* @brief Absolute path to the engine internal assets directory. */
		static inline FilePath sInternalAssetsRoot = detail::fs::current_path( ).parent_path( ) / "LumEngine" / "InternalAssets";

		/* @brief Path to the shared shader define header prepended to all shaders. */
		static inline FilePath sShaderDefineFile = detail::fs::current_path( ).parent_path( ) / "LumEngine" / "Engine" / "Include" / "Modules" / "Render" / "ShadersDefine.h";

		/* @brief Buffer storing the last asset load error message. */
		static inline char sLastErrorMessage[ limits::kMaxErrorAssetLoadLength ]{};

		/* @brief Sets the error message from a string literal. */
		template<usize tLength>
		static void set_error_msg( const char( &msg )[ tLength ] ) {
			usize length = (tLength < limits::kMaxErrorAssetLoadLength) ? tLength : limits::kMaxErrorAssetLoadLength;
			std::strncpy( sLastErrorMessage, msg, length );
		}

		/* @brief Sets the error message from a runtime string. */
		static void set_error_msg( const String& msg ) {
			std::strncpy( sLastErrorMessage, msg.data( ), sizeof( sLastErrorMessage ) );
		}

		static String get_full_path( ResourceRoot root, StringView filepath ) {
			if (root == ResourceRoot::External)
				return (sProjectRoot / filepath).lexically_normal( ).string( );
			else if (root == ResourceRoot::Internal)
				return (sInternalAssetsRoot / filepath).lexically_normal( ).string( );
			return "";
		}

		ResourceLoader( const ResourceLoader& ) = delete;
		ResourceLoader( ResourceLoader&& ) = delete;
		ResourceLoader( ) = default;

	};
} // namespace lum