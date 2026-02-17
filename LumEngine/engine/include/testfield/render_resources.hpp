#pragma once
#include "rhi/rhi_common.hpp"

namespace lum {

	namespace detail {
		struct RenderResources {

			rhi::RBufferHandle mVbo;
			rhi::RBufferHandle mEbo;
			rhi::RVertexLayoutHandle mVao;
			rhi::RTextureHandle mTexture;
			uint16 mTextureBinding = 0;
			rhi::RSamplerHandle mSampler;
			uint16 mSamplerBinding = 0;
			rhi::RPipelineHandle mPipeline;

		};
	}

}