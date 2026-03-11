//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Shader resource management.
//          Provides loading and caching of vertex and fragment shaders
//          to avoid redundant GPU resource creation.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"
#include "core/utils/handle_pool.hpp"

namespace lum {

	namespace rhi {
		class RenderDevice;
		struct RShaderHandle;
	} // lum::rhi

	/* @brief Manages loading and caching of GPU shaders.
	*
	* Provides a centralized shader management system with built-in caching.
	* Shaders are identified by a hash of their file paths to avoid redundant
	* loading and compilation.
	*/
	class MShaderManager {
	public:

		/* @brief Default constructor.
		*/
		MShaderManager( ) {}

		/* @brief Initializes the shader manager with a render device.
		* @param device Pointer to the RHI render device used for shader creation.
		*/
		void Initialize( rhi::RenderDevice* device );

		/* @brief Loads a shader program from vertex and fragment source files.
		*
		* If a shader with the given file paths has already been loaded,
		* the cached handle is returned immediately without reloading.
		* Fatally logs and breaks on load failure.
		*
		* @param vertexPath   Path to the vertex shader source file.
		* @param fragmentPath Path to the fragment shader source file.
		* @param root         Root directory identifier for asset resolution. Defaults to ERootID::External.
		* @return             Handle to the loaded (or cached) shader program.
		*/
		rhi::RShaderHandle LoadShader( ccharptr vertexPath, ccharptr fragmentPath, RootID root = RootID::External );

	private:

		/* @brief Pointer to the RHI device used for shader creation. */
		rhi::RenderDevice* mRenderDevice = nullptr;

		/* @brief Cache mapping shader path hashes to their corresponding shader handles. */
		std::unordered_map<uint64, rhi::RShaderHandle> mShaders;

	};
} // namespace lum