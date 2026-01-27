#pragma once
#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"
namespace lum::rhi {

	struct alignas(2) ShaderHandle {

		ShaderID id = max_val<ShaderID>();
		ShaderID generation = 0;

	};

	struct ShaderDescriptor {
		std::string vertex_source;
		std::string fragment_source;
	};

	struct Shader {

		ShaderID handle = 0;

	};

}