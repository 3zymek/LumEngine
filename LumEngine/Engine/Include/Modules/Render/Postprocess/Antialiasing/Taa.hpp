#pragma once

#include "Render/RenderCommon.hpp"

namespace lum::render {

	class TemporalAntiAliasing {
	public:

		void Initialize( rhi::IRenderDevice& dev );
		void EnsureFrameTex( const rhi::TextureCreateInfo& desc );
		rhi::TextureHandle GetPreviousFrameTex( ) const noexcept { return m_PreviousFrameTex; }
		Matrix4 ApplyJitter( const Matrix4& projection );

		Vector2 m_CurrentJitter{};

	private:

		SafePtr<rhi::IRenderDevice> m_RenderDev = nullptr;

		uint32 m_FrameIndex = 0;
		rhi::TextureHandle m_PreviousFrameTex;

		uint32 m_Width = 0;
		uint32 m_Height = 0;

	};

}