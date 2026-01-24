#pragma once
#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"
namespace lum::rhi {

	struct alignas(2) ShaderHandle {

		shaderid id = max_val<shaderid>();
		shaderid generation = 0;

	};

	struct ShaderDescriptor {
		std::string vertex_source;
		std::string fragment_source;
	};

	struct Shader {

		shaderid handle = 0;

	};

}