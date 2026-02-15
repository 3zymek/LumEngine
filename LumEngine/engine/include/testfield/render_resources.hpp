#pragma once
#include "rhi/rhi_common.hpp"

namespace lum {

	namespace detail {
		struct RenderResources {

			rhi::BufferHandle mVbo;
			rhi::BufferHandle mEbo;
			rhi::VertexLayoutHandle mVao;
			rhi::TextureHandle mTexture;
			uint16 mTextureBinding = 0;
			rhi::SamplerHandle mSampler;
			uint16 mSamplerBinding = 0;
			rhi::PipelineHandle mPipeline;

		};
	}

}