//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Texture configuration structures (descriptors, formats, types)
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	struct TextureRect {
		uint32 x = 0;
		uint32 y = 0;
		
		uint32 mWidth = 0;
		uint32 mHeight = 0;
		uint32 mDepth = 1;

		uint32 mMipLevel = 0;
	};

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
		ccharptr mFilename = nullptr;
		cvptr mData = nullptr; // Data with texture ( higher priority than 'filename' )

		InternalImageFormat mInternalFormat = InternalImageFormat::RGBA8; // GPU storage format

		LoadedImageFormat mLoadedFormat = LoadedImageFormat::RGBA; // CPU layout
		TextureDataType mDataType = TextureDataType::UnsignedByte; // CPU channel type

		bool bCompressed = false;

		bool bFlipVertically = false;
		bool bPremultiplyAlpha = false;

		// Can texture have LOD ( Level Of Detail )
		bool bGenerateMipmaps = false;
		uint32 mMipmapLevels = 0; // Set 0 to calculate automatically

		uint32 mSamples = 0;

		TextureRect mRect;

	};

	struct TextureCubemapDescriptor {
		
		InternalImageFormat mInternalFormat = InternalImageFormat::RGBA8;

		LoadedImageFormat mDataFormat = LoadedImageFormat::RGBA;
		TextureDataType mDataType = TextureDataType::UnsignedByte;

		bool bGenerateMipmaps = false;
		uint32 mMipmapLevels = 1;

		bool bFlipVertically = false;
		bool bPremultiplyAlpha = false;

		ccharptr mFaces[6]{};

	};

	struct Texture {

		TextureRect mRect;

		TextureType mType;

		InternalImageFormat mInternalFormat = InternalImageFormat::RGBA8;
		LoadedImageFormat mDataFormat = LoadedImageFormat::RGBA;
		TextureDataType mDataType = TextureDataType::UnsignedByte;

		uint32 mMipmapLevels = 0;

		union {
			TextureID gl = 0;
		} mHandle{};

	};

}
