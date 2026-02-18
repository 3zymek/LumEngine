#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
	// TO IMPLEMENT:
	void GLDevice::cache_uniforms_locations() {




	}

	bool GLDevice::is_valid_buffer_descriptor(const RBufferDescriptor& desc) noexcept {

		if (desc.mBufferUsage == RBufferUsage::Static) {

			if (!desc.mMapFlags.HasOnly(RMapFlag::Read | RMapFlag::Coherent)) {
				LUM_LOG_ERROR("Invalid buffer descriptor - Static buffers only allow Read and Coherent flags");
				return false;
			}

		}
		else if ((desc.mMapFlags.Has(RMapFlag::Coherent)) && !(desc.mMapFlags.Has(RMapFlag::Persistent))) {
			LUM_LOG_ERROR("Invalid buffer descriptor");
			return false;
		}

		return true;

	}

	GLbitfield GLDevice::translate_mapping_flags(Flags<RMapFlag> flags) noexcept {
		GLbitfield flag = 0;

		if (flags.Has(RMapFlag::Persistent))				flag |= GL_MAP_PERSISTENT_BIT;
		if (flags.Has(RMapFlag::Write))					flag |= GL_MAP_WRITE_BIT;
		if (flags.Has(RMapFlag::Read))					flag |= GL_MAP_READ_BIT;
		if (flags.Has(RMapFlag::Coherent))				flag |= GL_MAP_COHERENT_BIT;
		if (flags.Has(RMapFlag::Invalidate_Buffer))		flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags.Has(RMapFlag::Invalidate_Range))		flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags.Has(RMapFlag::Unsynchronized))			flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}
}