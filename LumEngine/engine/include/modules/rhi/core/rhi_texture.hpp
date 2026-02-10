#pragma once
#include "rhi/rhi_pch.hpp"
#include "rhi/rhi_common.hpp"
#include "core/core_common.hpp"
#include "core/shaders_define.h"
namespace lum::rhi {

	enum class TextureType : uint8 {
		Texture1D,
		Texture2D,
		Texture3D,
		Texture2DArray,
		Texture2DMultisample,
		Cubemap,
		CubemapArray
	};

	enum class InternalImageFormat : uint8 {

		// 8-bit normalized
		RGBA8,
		SRGB8_Alpha8,
		RGB8,
		RG8,
		R8,

		// 16-bit float (HDR)
		RGBA16F,
		RGB16F,
		RG16F,
		R16F,

		// 32-bit float
		RGBA32F,
		RGB32F,
		RG32F,
		R32F,

		// Depth/Stencil
		Depth24,
		Depth32F,
		Depth24Stencil8,
		Depth32FStencil8,
	};

	enum class LoadedImageFormat : uint8 {
		RGBA,
		RGB,
		RG,
		R,
		BGRA,
		DepthComponent,
		DepthStencil
	};

	enum class TextureDataType : uint8 {
		UnsignedByte,
		Byte,

		UnsignedShort,
		Short,

		UnsignedInt,
		Int,

		HalfFloat,
		Float,

		// Packed formats
		UnsignedInt_24_8,
		Float_32_UnsignedInt_24_8_Rev
	};

	struct TextureDescriptor {

		// Path/Name of the texture
		ccharptr filename = nullptr;
		cvptr data = nullptr; // Data with texture ( higher priority than 'filename' )

		InternalImageFormat internalFormat = InternalImageFormat::RGBA8; // GPU storage format

		LoadedImageFormat dataFormat = LoadedImageFormat::RGBA; // CPU layout
		TextureDataType dataType = TextureDataType::UnsignedByte; // CPU channel type

		bool bCompressed = false;

		bool bFlipVertically = false;
		bool premultiplyAlpha = false;

		// Can texture have LOD ( Level Of Detail )
		bool bGenerateMipmaps = false;
		uint32 mipmapLevels = 0; // If 0 - calculate automatically

		uint32 samples = 0;

		uint32 width = 0; // leave 0 if width needs to be loaded from texture
		uint32 height = 0; // leave 0 if height needs to be loaded from texture
		uint32 depth = 1;

	};

	struct TextureRegion {

		uint32 x = 0;
		uint32 y = 0;
		uint32 z = 0;

		uint32 width = 0;
		uint32 height = 0;
		uint32 depth = 1;

		uint32 mip = 0;

	};

	struct TextureCubemapDescriptor {
		
		InternalImageFormat internalFormat = InternalImageFormat::RGBA8;

		LoadedImageFormat dataFormat = LoadedImageFormat::RGBA;
		TextureDataType dataType = TextureDataType::UnsignedByte;

		bool bGenerateMipmaps = false;
		uint32 mipmapLevels = 1;

		bool bFlipVertically = false;
		bool bPremultiplyAlpha = false;

		ccharptr faces[6]{};

	};

	struct Texture {

		uint32 width = 0;
		uint32 height = 0;
		uint32 depth = 1;

		TextureType type;

		union {
			TextureID glHandle = 0;
		} handle{};

	};

}
