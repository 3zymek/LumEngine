#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
	// TO IMPLEMENT:
	void GLDevice::cache_uniforms_locations() {




	}

	bool GLDevice::is_valid_buffer_descriptor(const BufferDescriptor& desc) noexcept {

		if (desc.bufferUsage == BufferUsage::Static) {

			if (!desc.mapFlags.has_only(Mapflag::Read | Mapflag::Coherent)) {
				LUM_LOG_ERROR("Invalid buffer descriptor - Static buffers only allow Read and Coherent flags");
				return false;
			}

		}
		else if ((desc.mapFlags.has(Mapflag::Coherent)) && !(desc.mapFlags.has(Mapflag::Persistent))) {
			LUM_LOG_ERROR("Invalid buffer descriptor");
			return false;
		}

		return true;

	}

	GLbitfield GLDevice::translate_mapping_flags(Flags<Mapflag> flags) noexcept {
		GLbitfield flag = 0;

		if (flags.has(Mapflag::Persistent))				flag |= GL_MAP_PERSISTENT_BIT;
		if (flags.has(Mapflag::Write))					flag |= GL_MAP_WRITE_BIT;
		if (flags.has(Mapflag::Read))					flag |= GL_MAP_READ_BIT;
		if (flags.has(Mapflag::Coherent))				flag |= GL_MAP_COHERENT_BIT;
		if (flags.has(Mapflag::Invalidate_Buffer))		flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags.has(Mapflag::Invalidate_Range))		flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags.has(Mapflag::Unsynchronized))			flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}
}