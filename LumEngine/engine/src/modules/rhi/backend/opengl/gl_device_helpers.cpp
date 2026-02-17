#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
	// TO IMPLEMENT:
	void GLDevice::cache_uniforms_locations() {




	}

	bool GLDevice::is_valid_buffer_descriptor(const RBufferDescriptor& desc) noexcept {

		if (desc.mBufferUsage == RBufferUsage::Static) {

			if (!desc.mMapFlags.HasOnly(RMapflag::Read | RMapflag::Coherent)) {
				LUM_LOG_ERROR("Invalid buffer descriptor - Static buffers only allow Read and Coherent flags");
				return false;
			}

		}
		else if ((desc.mMapFlags.Has(RMapflag::Coherent)) && !(desc.mMapFlags.Has(RMapflag::Persistent))) {
			LUM_LOG_ERROR("Invalid buffer descriptor");
			return false;
		}

		return true;

	}

	GLbitfield GLDevice::translate_mapping_flags(Flags<RMapflag> flags) noexcept {
		GLbitfield flag = 0;

		if (flags.Has(RMapflag::Persistent))				flag |= GL_MAP_PERSISTENT_BIT;
		if (flags.Has(RMapflag::Write))					flag |= GL_MAP_WRITE_BIT;
		if (flags.Has(RMapflag::Read))					flag |= GL_MAP_READ_BIT;
		if (flags.Has(RMapflag::Coherent))				flag |= GL_MAP_COHERENT_BIT;
		if (flags.Has(RMapflag::Invalidate_Buffer))		flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags.Has(RMapflag::Invalidate_Range))		flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags.Has(RMapflag::Unsynchronized))			flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}
}