#pragma once
#include "rhi/rhi_common.hpp"
#include "core/shaders_define.h"
namespace lum::rhi {

	struct alignas(8) SamplerHandle {

		SamplerID id = max_val<SamplerID>();
		SamplerID generation = 0;

	};

	// Filter that applies to texture, when texture is magnified ( scaled up )
	enum class SamplerMagFilter : byte {
		Linear,	// Takes 4 neighbouring texels and calculates it weighted average value (Effect: Smoothness, Fuzzy, no sharp edges).
		Nearest	// Takes texture coordinate from UV map, rounds it and takes 1 texel (Effect: Sharpness). 
	};

	// Filter that applies to texture, when texture is minified ( scaled down )
	enum class SamplerMinFilter : byte {
		Linear,					// Takes 4 neighbouring texels and calculates it weighted average value ( Effect: Smoothness, Fuzzy, no sharp edges, NO MIPMAP ).
		LinearMipmapNearest,	// Takes 4 neighbouring texels from single mipmap and calculates it weighted average value ( Effect: Compromise for quality and performance, smoothness ).
		LinearMipmapLinear,	// Takes 4 neighbouring texels from 2 mipmaps and calculates it weighted average value ( Effect: Best quality, smooth but not fuzzy ).
		Nearest,				// Takes texture coordinate from UV map, rounds it and takes 1 texel ( Effect: Sharpness, NO MIPMAP ). 
		NearestMipmapNearest, // Takes 1 texel from single mipmap ( Effect: Even more sharpness, worse quality ).
		NearestMipmapLinear	// Takes 1 texel from 2 mipmaps and calculates it weighet average value ( Effect: Low detailed, weird look ).
	};

	// Defines what will happen to texture when out of bounds
	enum class SamplerWrap : byte {
		Repeat,				// Texture will repeat
		RepeatMirrored,	// Texture will repeat and every second repeat is mirrored
		ClampEdge,			// Last texture pixel will be streched
		ClampBorder		// Sets color border outside bounds
	};

	struct SamplerDescriptor {

		// Filter that applies to texture, when texture is magnified ( scaled up )
		SamplerMagFilter magFilter{};
		// Filter that applies to texture, when texture is minified ( scaled down )
		SamplerMinFilter minFilter{};

		// How texture is acting out of bounds on s( x ) axis ( when s > 1 )
		SamplerWrap wrapS{};
		// How texture is acting out of bounds on t( y ) axis ( when t > 1 )
		SamplerWrap wrapT{};
		// How texture is acting out of bounds on r( z ) axis ( when r > 1, ONLY FOR 3D TEXTURES )
		SamplerWrap wrapR{};

		// Level of anisotropic filtering ( sharpens textures at steep viewing angles, prevents blur and stretch )
		float32 anisotropy = 0.0f;

		bool bEnabled = false;
		bool b_enabled = false;

	};

	struct alignas(8) Sampler {

		SamplerID handle = 0;
		uint16 binding = LUM_NULL_BINDING;

	};

}