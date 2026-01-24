#pragma once
#include "rhi/rhi_common.hpp"
#include "core/shaders_define.h"
namespace lum::rhi {

	struct alignas(8) SamplerHandle {

		samplerid id = max_val<samplerid>();
		samplerid generation = 0;

	};

	// Filter that applies to texture, when texture is magnified ( scaled up )
	enum class SamplerMagFilter : byte {
		linear,	// Takes 4 neighbouring texels and calculates it weighted average value (Effect: Smoothness, Fuzzy, no sharp edges).
		nearest	// Takes texture coordinate from UV map, rounds it and takes 1 texel (Effect: Sharpness). 
	};

	// Filter that applies to texture, when texture is minified ( scaled down )
	enum class SamplerMinFilter : byte {
		linear,					// Takes 4 neighbouring texels and calculates it weighted average value ( Effect: Smoothness, Fuzzy, no sharp edges, NO MIPMAP ).
		linear_mipmap_nearest,	// Takes 4 neighbouring texels from single mipmap and calculates it weighted average value ( Effect: Compromise for quality and performance, smoothness ).
		linear_mipmap_linear,	// Takes 4 neighbouring texels from 2 mipmaps and calculates it weighted average value ( Effect: Best quality, smooth but not fuzzy ).
		nearest,				// Takes texture coordinate from UV map, rounds it and takes 1 texel ( Effect: Sharpness, NO MIPMAP ). 
		nearest_mipmap_nearest, // Takes 1 texel from single mipmap ( Effect: Even more sharpness, worse quality ).
		nearest_mipmap_linear	// Takes 1 texel from 2 mipmaps and calculates it weighet average value ( Effect: Low detailed, weird look ).
	};

	// Defines what will happen to texture when out of bounds
	enum class SamplerWrap : byte {
		repeat,				// Texture will repeat
		repeat_mirrored,	// Texture will repeat and every second repeat is mirrored
		clamp_edge,			// Last texture pixel will be streched
		clamp_border		// Sets color border outside bounds
	};

	struct SamplerDescriptor {

		// Filter that applies to texture, when texture is magnified ( scaled up )
		SamplerMagFilter mag_filter{};
		// Filter that applies to texture, when texture is minified ( scaled down )
		SamplerMinFilter min_filter{};

		// How texture is acting out of bounds on s( x ) axis ( when s > 1 )
		SamplerWrap wrap_s{};
		// How texture is acting out of bounds on t( y ) axis ( when t > 1 )
		SamplerWrap wrap_t{};
		// How texture is acting out of bounds on r( z ) axis ( when r > 1, ONLY FOR 3D TEXTURES )
		SamplerWrap wrap_r{};

		// Level of anisotropic filtering ( sharpens textures at steep viewing angles, prevents blur and stretch )
		float32 anisotropy = 0.0f;

	};

	struct alignas(8) Sampler {

		samplerid handle = 0;
		uint16 binding = LUM_NULL_BINDING;

	};

}