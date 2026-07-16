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

	void RenderDevice::Finalize( ) {

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

	bool RenderDevice::validate_texture_descriptor( const TextureCreateInfo& desc ) const noexcept {

		LUM_HOTCHK_RETURN_CUSTOM(
			mTextures.DenseSize( ) <= skMaxTextures,
			LUM_SEV_WARN,
			false,
			"Max textures reached"
		);

		LUM_HOTCHK_RETURN_CUSTOM(
			desc.mData.mPixels.data( ) != nullptr,
			LUM_SEV_WARN,
			false,
			"Texture pixel data is null"
		);

		return true;

	}
	bool RenderDevice::validate_buffer_descriptor( const BufferCreateInfo& desc ) const noexcept {

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

		LUM_HOTCHK_RETURN_CUSTOM(
			mBuffers.DenseSize( ) < skMaxBuffers,
			LUM_SEV_ERROR,
			false,
			"Max buffers reached"
		);

		LUM_ASSERT( desc.mSize > 0, "Invalid buffer size" );
		LUM_ASSERT( desc.mBufferType != BufferType::None, "No buffer type given" );

		return true;

	}

	bool RenderDevice::is_depth_format( TextureInternalFormat fmt ) const noexcept {

		return fmt == TextureInternalFormat::Depth16 ||
			fmt == TextureInternalFormat::Depth24 ||
			fmt == TextureInternalFormat::Depth32 ||
			fmt == TextureInternalFormat::Depth32F ||
			fmt == TextureInternalFormat::Depth24Stencil8 ||
			fmt == TextureInternalFormat::Depth32FStencil8;

	}
	bool RenderDevice::is_stencil_format( TextureInternalFormat fmt ) const noexcept {

		return fmt == TextureInternalFormat::StencilIndex8 ||
			fmt == TextureInternalFormat::Depth24Stencil8 ||
			fmt == TextureInternalFormat::Depth32FStencil8;

	}
	bool RenderDevice::is_color_format( TextureInternalFormat fmt ) const noexcept {

		return !is_depth_format( fmt ) && !is_stencil_format( fmt );

	}

	RenderDevice* CreateDevice( RenderBackend backend ) {

		rhi::RenderDevice* device = nullptr;

		switch (backend) {
		case RenderBackend::OpenGL: device = new gl::GLDevice( );
		}

		return device;
	}

}