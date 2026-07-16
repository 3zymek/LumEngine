#pragma once

#include "Render/RenderCommon.hpp"

namespace lum::render {

	class TemporalAntiAliasing {
	public:

		void Initialize( rhi::RenderDevice* dev );
		void EnsureFrameTex( const rhi::TextureCreateInfo& desc );
		rhi::TextureHandle GetPreviousFrameTex( ) const noexcept { return mPreviousFrameTex; }
		Matrix4 ApplyJitter( const Matrix4& projection );

		Vector2 mCurrentJitter{};

	private:

		rhi::RenderDevice* mRenderDev = nullptr;

		uint32 mFrameIndex = 0;
		rhi::TextureHandle mPreviousFrameTex;

		uint32 mWidth = 0;
		uint32 mHeight = 0;

	};

}