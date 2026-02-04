#pragma once
#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"
namespace lum::rhi {

	struct alignas(2) ShaderHandle {

		ShaderID id = null_id<ShaderID>();
		ShaderID generation = 0;

	};

	struct ShaderDescriptor {
		ccharptr vertexSource;
		ccharptr fragmentSource;
	};

	struct Shader {

		ShaderID handle = 0;

	};

}