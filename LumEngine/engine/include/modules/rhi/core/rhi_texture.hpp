#pragma once
#include "rhi/rhi_pch.hpp"
#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"
namespace lum::rhi {

	struct TextureHandle {

		uint64_t id = max_val<uint64_t>();
		uint32_t generation = 0;

	};

	struct TextureDescriptor {

		// Path/Name of the texture
		cstr_t filename = nullptr;

	};

	struct Texture {

		unsigned int width = 0;
		unsigned int height = 0;

		union {
			uint32_t gl_handle = 0;
		} handle{};

	};

}
