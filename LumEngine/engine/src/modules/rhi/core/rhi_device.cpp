//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Core of Render Hardware Interface.
//          Provides a backend-agnostic GPU abstraction for resource
//          creation, pipeline state management, and draw call submission.
//
//=============================================================================//

#include "rhi/core/rhi_device.hpp"
#include "platform/window.hpp"
#include "modules/rhi/backend/gl_device.hpp"

namespace lum::rhi {

	bool RenderDevice::validate_texture_descriptor(const FTextureDescriptor& desc) const noexcept {

		LUM_HOTCHK_RETURN_CUSTOM(
			mTextures.DenseSize() <= skMaxTextures,
			LUM_SEV_WARN,
			false,
			"Max textures reached"
		);

		LUM_HOTCHK_RETURN_CUSTOM(
			desc.mData.mPixels.data() != nullptr,
			LUM_SEV_WARN,
			false,
			"Texture pixel data is null"
		);

		return true;

	}
	bool RenderDevice::validate_buffer_descriptor( const FBufferDescriptor& desc ) const noexcept {

		if (desc.mBufferUsage == BufferUsage::Static) {
			LUM_ASSERT(
				desc.mMapFlags.IsEmpty(),
				"Static buffers cannot be mapped"
			);
		}

		if (desc.mMapFlags.Has(MapFlag::Coherent)) {
			LUM_ASSERT(
				desc.mMapFlags.Has(MapFlag::Persistent),
				"Coherent flag requires Persistent flag"
			);
		}

		if (desc.mMapFlags.Has(MapFlag::Persistent)) {
			LUM_ASSERT(
				desc.mMapFlags.Has(MapFlag::Read) || desc.mMapFlags.Has(MapFlag::Write),
				"Persistent flag requires Read or Write flag"
			);
		}

		LUM_HOTCHK_RETURN_CUSTOM(
			mBuffers.DenseSize() < skMaxBuffers,
			LUM_SEV_ERROR,
			false,
			"Max buffers reached"
		);

		LUM_ASSERT(desc.mSize > 0, "Invalid buffer size");
		LUM_ASSERT(desc.mBufferType != BufferType::None, "No buffer type given");

		return true;

	}

	bool RenderDevice::is_depth_format( ImageLayout fmt ) const noexcept {

		return fmt == ImageLayout::Depth16 ||
			fmt == ImageLayout::Depth24 ||
			fmt == ImageLayout::Depth32 ||
			fmt == ImageLayout::Depth32F ||
			fmt == ImageLayout::Depth24Stencil8 ||
			fmt == ImageLayout::Depth32FStencil8;

	}
	bool RenderDevice::is_stencil_format( ImageLayout fmt ) const noexcept {

		return fmt == ImageLayout::StencilIndex8 ||
			fmt == ImageLayout::Depth24Stencil8 ||
			fmt == ImageLayout::Depth32FStencil8;

	}
	bool RenderDevice::is_color_format( ImageLayout fmt ) const noexcept {

		return !is_depth_format(fmt) && !is_stencil_format(fmt);

	}

	RenderDevice* CreateDevice( Window* window, RenderBackend backend ) {
		
		rhi::RenderDevice* device = nullptr;

		switch (backend) {
		case RenderBackend::OpenGL: device = new gl::GLDevice();
		}

		device->Initialize(window);
	
		return device;
	}

}