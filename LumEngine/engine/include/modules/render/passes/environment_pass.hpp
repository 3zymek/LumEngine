//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Environment render pass — renders the HDR skybox cubemap
//          and precomputes IBL (Image-Based Lighting) maps:
//          irradiance map and prefiltered environment map.
//
//=============================================================================//
#pragma once

#include "render/render_common.hpp"

namespace lum::render {

	namespace detail { class GBuffer; }
	namespace detail {

		enum class IBLTexture : byte {
			IrradianceMap,
			PrefilteredMap
		};

	}

	/* @brief Handles skybox rendering and precomputation of IBL textures.
	*  On SetCubemapTexture(), generates the irradiance map (diffuse IBL)
	*  and prefiltered environment map (specular IBL) from the provided HDR cubemap.
	*/
	class EnvironmentPass {
	public:

		EnvironmentPass( ) = default;

		/* @brief Initializes the pass, allocates GPU resources and compiles shaders.
		*  @param ctx Context struct containing valid pointers to all subsystem managers.
		*/
		void Initialize( const FRendererContext& ctx );

		/* @brief Sets the HDR cubemap texture and triggers IBL precomputation.
		*  Must be called before Execute() for IBL to work correctly.
		*  @param tex Handle to a valid HDR cubemap texture.
		*/
		void SetCubemapTexture( rhi::RTextureHandle tex ) {
			mCubemap.mTexture = tex;
			generate_irradiance_map( );
			generate_prefiltered_map( );
		}

		/* @brief Renders the skybox cubemap to the provided screen quad framebuffer.
		*  @param gbuffer  GBuffer containing depth for correct skybox depth testing.
		*  @param quad     Screen quad FBO to render into.
		*/
		void Execute( detail::GBuffer& gbuffer, const detail::FScreenQuad& quad );

		/* @brief Returns a handle to a precomputed IBL texture.
		*  @param tex Which IBL texture to retrieve.
		*/
		rhi::RTextureHandle GetTexture( detail::IBLTexture tex ) const noexcept;

	private:

		/* @brief Cached context holding all subsystem manager references. */
		FRendererContext mContext;

		/* @brief Cubemap mesh, shader, pipeline and texture data. */
		detail::FCubemap mCubemap;

		/* @brief Pipeline state for skybox rendering. */
		rhi::RPipelineHandle mPipeline;

		/* @brief Shader program for skybox rendering. */
		rhi::RShaderHandle mShader;

		/* @brief Sampler used for cubemap and IBL texture sampling. */
		rhi::RSamplerHandle mSampler;

		/* @brief Contains all IBL precomputed textures and their generation shaders. */
		struct FIBL {

			/* @brief Irradiance cubemap — encodes diffuse indirect lighting
			*  by convolving the environment over the hemisphere. 32x32 is sufficient.
			*/
			struct FIrradianceMap {

				rhi::RTextureHandle mTexture;
				rhi::RShaderHandle  mShader;

			} mIrradiance;

			/* @brief Prefiltered environment cubemap — encodes specular indirect lighting
			*  at varying roughness levels across mip chain (mip 0 = mirror, last mip = fully rough).
			*/
			struct FPrefilteredMap {

				rhi::RTextureHandle mTexture;
				rhi::RShaderHandle  mShader;
				static constexpr uint8 skMipmapLevels = 5;

			} mPrefiltered;

			static glm::mat4 GetCaptureProjection( ) {
				return glm::perspective( glm::radians( 90.0f ), 1.0f, 0.1f, 10.0f );
			}

			static std::array<glm::mat4, 6> GetCaptureViews( ) {
				return { {
					glm::lookAt( glm::vec3( 0 ), glm::vec3( 1,  0,  0 ), glm::vec3( 0, -1,  0 ) ),
					glm::lookAt( glm::vec3( 0 ), glm::vec3( -1,  0,  0 ), glm::vec3( 0, -1,  0 ) ),
					glm::lookAt( glm::vec3( 0 ), glm::vec3( 0,  1,  0 ), glm::vec3( 0,  0,  1 ) ),
					glm::lookAt( glm::vec3( 0 ), glm::vec3( 0, -1,  0 ), glm::vec3( 0,  0, -1 ) ),
					glm::lookAt( glm::vec3( 0 ), glm::vec3( 0,  0,  1 ), glm::vec3( 0, -1,  0 ) ),
					glm::lookAt( glm::vec3( 0 ), glm::vec3( 0,  0, -1 ), glm::vec3( 0, -1,  0 ) ),
				} };
			}

		} mIBL;

		/* @brief Convolves the HDR environment into a 32x32 irradiance cubemap. */
		void generate_irradiance_map( );

		/* @brief Prefilters the HDR environment into a mipmapped cubemap for specular IBL. */
		void generate_prefiltered_map( );

		/* @brief Allocates all GPU resources — buffers, textures, samplers, pipelines, shaders. */
		void init( );

	};

} // namespace lum::render