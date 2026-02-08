#pragma once
#include "rhi/rhi_pch.hpp"
#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"
#include "core/shaders_define.h"
namespace lum::rhi {

	struct TextureDescriptor {

		// Path/Name of the texture
		ccharptr filename = nullptr;

		// Can texture have LOD ( Level Of Detail )
		bool mipmaping = false;

		uint32 width = 0;
		uint32 height = 0;

	};

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
