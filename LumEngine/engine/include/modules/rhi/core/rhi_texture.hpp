#pragma once
#include "rhi/rhi_pch.hpp"
#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"
namespace lum::rhi {

	struct TextureHandle {

		textureid id = max_val<textureid>();
		textureid generation = 0;

	};

	struct TextureDescriptor {

		// Path/Name of the texture
		LUMcharptr filename = nullptr;

		// Can texture have LOD ( Level Of Detail )
		bool mipmaping = false;

	};

	struct Texture {

		LUMuint width = 0;
		LUMuint height = 0;

		union {
			textureid gl_handle = 0;
		} handle{};

	};

}
