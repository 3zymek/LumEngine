#include "rhi/core/rhi_device.hpp"
#include "platform/window.hpp"
#include "rhi/backend/opengl/gl_device.hpp"
namespace lum::rhi {

	bool RDevice::validate_texture_descriptor(const RTextureDescriptor& desc) {

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
	bool RDevice::validate_buffer_descriptor(const FBufferDescriptor& desc) {

		if (desc.mBufferUsage == EBufferUsage::Static) {
			LUM_ASSERT(
				desc.mMapFlags.IsEmpty(),
				"Static buffers cannot be mapped"
			);
		}

		// Coherent wymaga Persistent
		if (desc.mMapFlags.Has(EMapFlag::Coherent)) {
			LUM_ASSERT(
				desc.mMapFlags.Has(EMapFlag::Persistent),
				"Coherent flag requires Persistent flag"
			);
		}

		// Persistent wymaga Read lub Write
		if (desc.mMapFlags.Has(EMapFlag::Persistent)) {
			LUM_ASSERT(
				desc.mMapFlags.Has(EMapFlag::Read) || desc.mMapFlags.Has(EMapFlag::Write),
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
		LUM_ASSERT(desc.mBufferType != EBufferType::None, "No buffer type given");

		return true;

	}

	bool RDevice::is_depth_format(RImageLayout fmt) {
		return fmt == RImageLayout::Depth16 ||
			fmt == RImageLayout::Depth24 ||
			fmt == RImageLayout::Depth32 ||
			fmt == RImageLayout::Depth32F ||
			fmt == RImageLayout::Depth24Stencil8 ||
			fmt == RImageLayout::Depth32FStencil8;
	}
	bool RDevice::is_stencil_format(RImageLayout fmt) {
		return fmt == RImageLayout::StencilIndex8 ||
			fmt == RImageLayout::Depth24Stencil8 ||
			fmt == RImageLayout::Depth32FStencil8;
	}
	bool RDevice::is_color_format(RImageLayout fmt) {
		return !is_depth_format(fmt) && !is_stencil_format(fmt);
	}

	RDevice* CreateDevice(Window* window) {
		RenderBackend backend = window->GetBackend();
		switch (backend) {
		case RenderBackend::OpenGL: return new gl::GLDevice(window);
		}
		return nullptr;
	}

}