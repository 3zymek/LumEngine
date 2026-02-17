//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Texture configuration structures (descriptors, formats, types)
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	struct RTextureRect {
		uint32 x = 0;
		uint32 y = 0;
		
		uint32 mWidth = 0;
		uint32 mHeight = 0;
		uint32 mDepth = 1;

		uint32 mMipLevel = 0;
	};

	enum class RTextureType : uint8 {
		Texture1D,
		Texture2D,
		Texture3D,
		Texture2DArray,
		Texture2DMultisample,
		Cubemap,
		CubemapArray
	};

	enum class RInternalImageFormat : uint8 {

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

	enum class RLoadedImageFormat : uint8 {
		RGBA,
		RGB,
		RG,
		R,
		BGRA,
		DepthComponent,
		DepthStencil
	};

	enum class RTextureDataType : uint8 {
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

	struct RTextureDescriptor {

		FTextureData mData;

		RInternalImageFormat mInternalFormat = RInternalImageFormat::RGBA8; // GPU storage format

		RLoadedImageFormat mLoadedFormat = RLoadedImageFormat::RGBA; // CPU layout
		RTextureDataType mDataType = RTextureDataType::UnsignedByte; // CPU channel type

		//bool bCompressed = false;

		//bool bFlipVertically = false;
		//bool bPremultiplyAlpha = false;

		// Can texture have LOD ( Level Of Detail )
		bool bGenerateMipmaps = false;
		uint32 mMipmapLevels = 0; // Set 0 to calculate automatically

		uint32 mSamples = 0;

	};

	struct RTextureUpdateDescriptor {

		RTextureRect	mRect;
		FTextureData		mData;
		bool			bGenerateMipmaps = false;

	};

	struct RTextureCubemapDescriptor {
		
		RInternalImageFormat mInternalFormat = RInternalImageFormat::RGBA8;

		RLoadedImageFormat mDataFormat = RLoadedImageFormat::RGBA;
		RTextureDataType mDataType = RTextureDataType::UnsignedByte;

		bool bGenerateMipmaps = false;
		uint32 mMipmapLevels = 1;

		bool bFlipVertically = false;
		bool bPremultiplyAlpha = false;

		FTextureData mFaces[6]{};

	};

	struct RTexture {

		RTextureRect mRect;

		RTextureType mType;

		RInternalImageFormat mInternalFormat = RInternalImageFormat::RGBA8;
		RLoadedImageFormat mDataFormat = RLoadedImageFormat::RGBA;
		RTextureDataType mDataType = RTextureDataType::UnsignedByte;

		uint32 mMipmapLevels = 0;

		union {
			RTextureID gl = 0;
		} mHandle{};

	};

}
