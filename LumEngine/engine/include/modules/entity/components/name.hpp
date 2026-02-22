#include "core/utils/fixed_string.hpp"
#include "entity/ecs_common.hpp"

namespace lum {

	struct CName {

		LUM_COMPONENT_TAG;

		FixedString<32> mName;

	};

} // namespace lum