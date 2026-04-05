#pragma once

#include "core/utils/handle_pool.hpp"

namespace lum {

	struct StaticMeshHandle : cstd::BaseHandle<uint32> { };
	struct DynamicMeshHandle : cstd::BaseHandle<uint32> { };

}