#pragma once

#include "render/common.hpp"

namespace lum::render {

	class GeometryPass;
	class LightPass;


	class ShadowPass {
	public:
		
		ShadowPass( ) = default;

		void Initialize( const FRendererContext& ctx );

		rhi::RTextureHandle GetShadowMap( ) const { return mShadowMap; }

		void Execute( GeometryPass& geometryPass, const LightPass& lightPass );

	private:

		FRendererContext mContext;

		float32 mShadowMapDistance = 50.f;
		float32 mShadowMapSize = 100.f;
		float32 mShadowMapNear = 0.1f;
		float32 mShadowMapFar = mShadowMapDistance * 2.0f;

		rhi::RTextureHandle mShadowMap;
		rhi::RFramebufferHandle mFramebuffer;
		rhi::RShaderHandle mShader;

		rhi::RBufferHandle mLightSpaceUniform;
		
		void calculate_lightspace_matrix( const glm::vec3& direction );

		void upload_lightspace_matrix( const glm::mat4& mat );

		void init( );

	};

} // namespace lum::render