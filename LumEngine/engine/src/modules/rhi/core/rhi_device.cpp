#include "rhi/core/rhi_device.hpp"
#include "platform/window.hpp"
#include "rhi/backend/opengl/gl_device.hpp"
namespace lum::rhi {

	bool RDevice::validate_texture_descriptor(const FTextureDescriptor& desc) {

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
	bool RDevice::validate_buffer_descriptor( const FBufferDescriptor& desc ) {

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

	bool RDevice::is_depth_format( ImageLayout fmt ) {

		return fmt == ImageLayout::Depth16 ||
			fmt == ImageLayout::Depth24 ||
			fmt == ImageLayout::Depth32 ||
			fmt == ImageLayout::Depth32F ||
			fmt == ImageLayout::Depth24Stencil8 ||
			fmt == ImageLayout::Depth32FStencil8;

	}
	bool RDevice::is_stencil_format( ImageLayout fmt ) {

		return fmt == ImageLayout::StencilIndex8 ||
			fmt == ImageLayout::Depth24Stencil8 ||
			fmt == ImageLayout::Depth32FStencil8;

	}
	bool RDevice::is_color_format( ImageLayout fmt ) {

		return !is_depth_format(fmt) && !is_stencil_format(fmt);

	}

	RDevice* CreateDevice( Window* window, RenderBackend backend ) {
		
		rhi::RDevice* device = nullptr;

		switch (backend) {
		case RenderBackend::OpenGL: device = new gl::GLDevice();
		}

		device->Initialize(window);
	
		return device;
	}

}