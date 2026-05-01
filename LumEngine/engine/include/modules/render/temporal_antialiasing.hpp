#pragma once

#include "render/render_common.hpp"

namespace lum::render {

	class TemporalAntiAliasing {
	public:

		void Initialize( rhi::RenderDevice* dev );
		void EnsureFrameTex( const rhi::FTextureDescriptor& desc );
		rhi::RTextureHandle GetPreviousFrameTex( ) const noexcept { return mPreviousFrameTex; }
		glm::mat4 ApplyJitter( const glm::mat4& projection );

	private:

		rhi::RenderDevice* mRenderDev = nullptr;

		uint32 mFrameIndex = 0;
		rhi::RTextureHandle mPreviousFrameTex;

		uint32 mWidth = 0;
		uint32 mHeight = 0;

	};

}