#pragma once
#include "rhi/rhi_pch.hpp"
#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"
#include "core/shaders_define.h"
namespace lum::rhi {

	struct alignas(8) TextureHandle {

		TextureID id = max_val<TextureID>();
		TextureID generation = 0;

	};

	struct TextureDescriptor {

		// Path/Name of the texture
		ccharptr filename = nullptr;

		// Can texture have LOD ( Level Of Detail )
		bool mipmaping = false;

		uint32 width = 0;
		uint32 height = 0;

	};

#define LUM_CUBEMAP_POSITIVE_X 0
#define LUM_CUBEMAP_NEGATIVE_X 1
#define LUM_CUBEMAP_POSITIVE_Y 2
#define LUM_CUBEMAP_NEGATIVE_Y 3
#define LUM_CUBEMAP_POSITIVE_Z 4
#define LUM_CUBEMAP_NEGATIVE_Z 5

	struct TextureCubemapDescriptor {
		
		ccharptr faces[6]{};

	};

	struct Texture {

		uint32 width = 0;
		uint32 height = 0;

		uint16 binding = LUM_NULL_BINDING;

		union {
			TextureID glHandle = 0;
		} handle{};

	};

}
