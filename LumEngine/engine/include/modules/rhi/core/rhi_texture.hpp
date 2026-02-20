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

	enum class RTextureType : byte {
		None,
		Texture2D,
		Texture3D,
		Cubemap
	};

	enum class RInternalImageFormat : byte {

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

	enum class RLoadedImageFormat : byte {
		RGBA,
		RGB,
		RG,
		R,
		BGRA,
		DepthComponent,
		DepthStencil
	};

	enum class RTextureDataType : byte {
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

	/* @brief Describes the properties of a texture to be created on the GPU.
	*
	* Contains all information required to upload and configure a texture,
	* including its pixel data, format, dimensions, and type. Width and height
	* can be omitted if they are already present in mData, in which case they
	* will be inferred automatically.
	*
	* @param mData              Raw pixel data loaded on the CPU side.
	* @param mInternalFormat    GPU-side storage format for the texture.
	* @param mLoadedFormat      CPU-side pixel channel layout.
	* @param mDataType          Data type of each pixel channel on the CPU.
	* @param bGenerateMipmaps   Whether to automatically generate mipmaps.
	* @param mMipmapLevels      Number of mip levels to generate (0 = automatic).
	* @param mSamples           Sample count for multisampled textures.
	* @param mTextureType       Dimensionality and type of the texture.
	* @param mWidth             Width in pixels (0 = infer from mData).
	* @param mHeight            Height in pixels (0 = infer from mData).
	* @param mDepth             Depth in pixels, used for Texture3D only.
	* @param mCubemap           Per-face pixel data, used for Cubemap only.
	*/
	struct RTextureDescriptor {

		FTextureData mData; // Texture data loaded from CPU

		RInternalImageFormat mInternalFormat = RInternalImageFormat::RGBA8; // GPU-side storage format
		RLoadedImageFormat mLoadedFormat = RLoadedImageFormat::RGBA;        // CPU-side pixel layout
		RTextureDataType mDataType = RTextureDataType::UnsignedByte;        // Data type of each channel on CPU

		bool bGenerateMipmaps = false; // Whether to auto-generate mipmaps

		uint32 mMipmapLevels = 0;// Number of mip levels (0 = calculate automatically)

		uint32 mSamples = 0; // Number of samples for multisampled textures

		RTextureType mTextureType = RTextureType::None; // Texture dimensionality and type

		uint32 mWidth = 0;  // Texture width in pixels (0 = infer from mData)
		uint32 mHeight = 0; // Texture height in pixels (0 = infer from mData)
		uint32 mDepth = 0;  // Texture depth in pixels, used for Texture3D only

		struct RCubemap {
			FTextureData mFaces[6]{}; // Pixel data for each cubemap face (+X, -X, +Y, -Y, +Z, -Z)
		} mCubemap;

	};

	struct RTextureUpdateDescriptor {

		RTextureRect	mRect;
		FTextureData	mData;
		bool			bGenerateMipmaps = false;

	};

	struct RTexture {

		RTextureRect mRect;

		RTextureType mType = RTextureType::None;

		RInternalImageFormat mInternalFormat = RInternalImageFormat::RGBA8;
		RLoadedImageFormat mDataFormat = RLoadedImageFormat::RGBA;
		RTextureDataType mDataType = RTextureDataType::UnsignedByte;

		uint32 mMipmapLevels = 0;

		RTextureID mHandle = 0;

	};

}
