#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	GLbitfield GLDevice::translate_mapping_flags(Flags<MapFlag> flags) noexcept {
		GLbitfield flag = 0;

		if (flags.Has(MapFlag::None))						return 0;
		if (flags.Has(MapFlag::Persistent))				flag |= GL_MAP_PERSISTENT_BIT;
		if (flags.Has(MapFlag::Write))						flag |= GL_MAP_WRITE_BIT;
		if (flags.Has(MapFlag::Read))						flag |= GL_MAP_READ_BIT;
		if (flags.Has(MapFlag::Coherent))					flag |= GL_MAP_COHERENT_BIT;
		if (flags.Has(MapFlag::Invalidate_Buffer))			flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags.Has(MapFlag::Invalidate_Range))			flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags.Has(MapFlag::Unsynchronized))			flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}
}