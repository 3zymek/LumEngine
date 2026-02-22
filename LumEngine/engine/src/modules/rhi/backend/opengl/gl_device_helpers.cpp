#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	GLbitfield GLDevice::translate_mapping_flags(Flags<EMapFlag> flags) noexcept {
		GLbitfield flag = 0;

		if (flags.Has(EMapFlag::Persistent))				flag |= GL_MAP_PERSISTENT_BIT;
		if (flags.Has(EMapFlag::Write))						flag |= GL_MAP_WRITE_BIT;
		if (flags.Has(EMapFlag::Read))						flag |= GL_MAP_READ_BIT;
		if (flags.Has(EMapFlag::Coherent))					flag |= GL_MAP_COHERENT_BIT;
		if (flags.Has(EMapFlag::Invalidate_Buffer))			flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags.Has(EMapFlag::Invalidate_Range))			flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags.Has(EMapFlag::Unsynchronized))			flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}
}