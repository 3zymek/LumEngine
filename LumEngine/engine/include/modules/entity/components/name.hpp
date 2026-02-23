#pragma once
#include "core/utils/fixed_string.hpp"
#include "entity/ecs_common.hpp"

namespace lum {

	struct CName : Component {

		FixedString<32> mName;

	};

} // namespace lum