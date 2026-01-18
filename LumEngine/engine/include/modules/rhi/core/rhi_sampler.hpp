#pragma once
#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	struct SamplerHandle {

	};

	// Filter that applies to texture, when texture is magnified ( scaled up )
	enum class TextureMagFilter : uint8_t {
		Linear,	// Takes 4 neighbouring texels and calculates it weighted average value (Effect: Smoothness, Fuzzy, no sharp edges).
		Nearest	// Takes texture coordinate from UV map, rounds it and takes 1 texel (Effect: Sharpness). 
	};

	// Filter that applies to texture, when texture is minified ( scaled down )
	enum class TextureMinFilter : uint8_t {
		Linear,					// Takes 4 neighbouring texels and calculates it weighted average value ( Effect: Smoothness, Fuzzy, no sharp edges, NO MIPMAP ).
		Linear_mipmap_nearest,	// Takes 4 neighbouring texels from single mipmap and calculates it weighted average value ( Effect: Compromise for quality and performance, smoothness ).
		Linear_mipmap_linear,	// Takes 4 neighbouring texels from 2 mipmaps and calculates it weighted average value ( Effect: Best quality, smooth but not fuzzy ).
		Nearest,				// Takes texture coordinate from UV map, rounds it and takes 1 texel ( Effect: Sharpness, NO MIPMAP ). 
		Nearest_mipmap_nearest, // Takes 1 texel from single mipmap ( Effect: Even more sharpness, worse quality ).
		Nearest_mipmap_linear	// Takes 1 texel from 2 mipmaps and calculates it weighet average value ( Effect: Low detailed, weird look ).
	};

	struct SamplerDescriptor {

		// Filter that applies to texture, when texture is magnified ( scaled up )
		TextureMagFilter mag_filter{};
		// Filter that applies to texture, when texture is minified ( scaled down )
		TextureMinFilter min_filter{};

		// Level of anisotropic filtering ( sharpens textures at steep viewing angles, prevents blur and stretch )
		int anisotropy = 0;

	};

	struct Sampler {

		TextureMagFilter mag_filter{};
		TextureMinFilter min_filter{};

		int anisotropy = 0;

		uint32_t handle = 0;

	};

}