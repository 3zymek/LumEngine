#pragma once
#include "rhi/rhi_pch.hpp"
#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"
namespace lum::rhi {

	struct TextureHandle {

		uint64_t id = max_val<uint64_t>();
		uint32_t generation = 0;

	};

	enum class TextureMagFilter : uint8_t {
		Linear,	// Takes 4 neighbouring texels and calculates it weighted average value (Effect: Smoothness, Fuzzy, no sharp edges).
		Nearest	// Takes texture coordinate from UV map, rounds it and takes 1 texel (Effect: Sharpness). 
	};

	enum class TextureMinFilter : uint8_t {
		Linear,					// Takes 4 neighbouring texels and calculates it weighted average value (Effect: Smoothness, Fuzzy, no sharp edges).
		Linear_mipmap_nearest,	// Takes 4 neighbouring texels from single mipmap and calculates it weighted average value (Effect: Old style games, smooth).
		Linear_mipmap_linear,	// Takes 4 neighbouring texels from 2 mipmaps and calculates it weighted average value (Effect: Best quality, smooth but not fuzzy).
		Nearest,				// Takes texture coordinate from UV map, rounds it and takes 1 texel (Effect: Sharpness). 
		Nearest_mipmap_nearest, // Takes 1 texel from single mipmap (Effect: Even more sharpness, worse quality).
		Nearest_mipmap_linear	// Takes 1 texel from 2 mipmaps and calculates it weighet average value (Effect: Low detailed, weird look).
	};

	struct TextureDescriptor {

		// Filter that applies to texture, when texture is magnified (scaled up)
		TextureMagFilter mag_filter;
		// Filter that applies to texture, when texture is minified (scaled down)
		TextureMinFilter min_filter;

		// Path/Name of the texture
		const char* filename;

	};

	struct Texture {

		// Filter that applies to texture, when texture is magnified (scaled up)
		TextureMagFilter mag_filters{};
		// Filter that applies to texture, when texture is minified (scaled down)
		TextureMinFilter min_filters{};

		unsigned int width = 0;
		unsigned int height = 0;

		union {
			uint32_t gl_handle = 0;
		} handle{};

	};

}
