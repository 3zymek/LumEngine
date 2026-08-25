//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core of Render Hardware Interface.
//          Provides a backend-agnostic GPU abstraction for resource
//          creation, pipeline state management, and draw call submission.
//
//=============================================================================//

#include "Rhi/Core/RhiDevice.hpp"
#include "Modules/Rhi/Backend/GlDevice.hpp"

namespace lum::rhi {

	void IRenderDevice::Finalize( ) {

		for (auto [handle, p] : mPipelines.Each( ))    Delete( handle );
		for (auto [handle, f] : mFramebuffers.Each( )) Delete( handle );
		for (auto [handle, l] : mLayouts.Each( ))      Delete( handle );
		for (auto [handle, b] : mBuffers.Each( ))      Delete( handle );
		for (auto [handle, t] : mTextures.Each( ))     Delete( handle );
		for (auto [handle, s] : mShaders.Each( ))      Delete( handle );
		for (auto [handle, l] : mSamplers.Each( ))     Delete( handle );

	}

	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	bool IRenderDevice::validate_texture_descriptor( const TextureCreateInfo& desc ) const noexcept {

		if (mTextures.DenseSize( ) >= skMaxTextures) {

			LUM_LOG_WARN( "Max textures reached" );
			return false;

		}

		if (desc.mData.mPixels.data( ) == nullptr) {
			LUM_LOG_WARN( "Texture pixel data is null" );
			return false;
		}

		return true;

	}
	bool IRenderDevice::validate_buffer_descriptor( const BufferCreateInfo& desc ) const noexcept {

		if (desc.mBufferUsage == BufferUsage::Static) {
			LUM_ASSERT(
				desc.mMapFlags.IsEmpty( ),
				"Static buffers cannot be mapped"
			);
		}

		if (desc.mMapFlags.Has( MapFlag::Coherent )) {
			LUM_ASSERT(
				desc.mMapFlags.Has( MapFlag::Persistent ),
				"Coherent flag requires Persistent flag"
			);
		}

		if (desc.mMapFlags.Has( MapFlag::Persistent )) {
			LUM_ASSERT(
				desc.mMapFlags.Has( MapFlag::Read ) || desc.mMapFlags.Has( MapFlag::Write ),
				"Persistent flag requires Read or Write flag"
			);
		}

		if (mBuffers.DenseSize( ) >= skMaxBuffers) {
			LUM_LOG_WARN( "Max buffers reached" );
			return false;
		}

		LUM_ASSERT( desc.mSize > 0, "Invalid buffer size" );
		LUM_ASSERT( desc.mBufferType != BufferType::None, "No buffer type given" );

		return true;

	}

	bool IRenderDevice::is_depth_format( TextureFormat fmt ) const noexcept {

		return fmt == TextureFormat::Depth16 ||
			fmt == TextureFormat::Depth24 ||
			fmt == TextureFormat::Depth32 ||
			fmt == TextureFormat::Depth32F ||
			fmt == TextureFormat::Depth24Stencil8 ||
			fmt == TextureFormat::Depth32FStencil8;

	}
	bool IRenderDevice::is_stencil_format( TextureFormat fmt ) const noexcept {

		return fmt == TextureFormat::StencilIndex8 ||
			fmt == TextureFormat::Depth24Stencil8 ||
			fmt == TextureFormat::Depth32FStencil8;

	}
	bool IRenderDevice::is_color_format( TextureFormat fmt ) const noexcept {

		return !is_depth_format( fmt ) && !is_stencil_format( fmt );

	}

	std::unique_ptr<IRenderDevice> CreateDevice( RenderBackend backend ) {

		switch (backend) {
			case RenderBackend::OpenGL:
				return std::move( std::make_unique<gl::GLDevice>( ) );
			default:
				return nullptr;
		}
	}

}