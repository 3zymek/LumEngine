#pragma once
#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"
namespace lum::rhi {

	struct ShaderHandle {

		uint64_t id = max_val<uint64_t>();
		uint32_t generation = 0;

	};

	struct ShaderDescriptor {
		std::string vertex_source;
		std::string fragment_source;
	};

	struct Shader {

		uint64_t handle = 0;

	};

}