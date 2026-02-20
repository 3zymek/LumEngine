#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	GLbitfield GLDevice::translate_mapping_flags(Flags<RMapFlag> flags) noexcept {
		GLbitfield flag = 0;

		if (flags.Has(RMapFlag::Persistent))				flag |= GL_MAP_PERSISTENT_BIT;
		if (flags.Has(RMapFlag::Write))						flag |= GL_MAP_WRITE_BIT;
		if (flags.Has(RMapFlag::Read))						flag |= GL_MAP_READ_BIT;
		if (flags.Has(RMapFlag::Coherent))					flag |= GL_MAP_COHERENT_BIT;
		if (flags.Has(RMapFlag::Invalidate_Buffer))			flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags.Has(RMapFlag::Invalidate_Range))			flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags.Has(RMapFlag::Unsynchronized))			flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}
}