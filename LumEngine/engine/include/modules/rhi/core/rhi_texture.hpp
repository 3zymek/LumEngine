//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Texture configuration structures (descriptors, formats, types)
//
//=============================================================================//
#pragma once
#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	/* @brief Defines a rectangular region within a texture, used for partial updates.
	* Also carries mip level and depth slice for 3D or array texture operations.
	*/
	struct RTextureRect {

		/* @brief X offset in pixels from the left edge of the texture. */
		uint32 x = 0;

		/* @brief Y offset in pixels from the top edge of the texture. */
		uint32 y = 0;

		/* @brief Width of the region in pixels. */
		uint32 mWidth = 0;

		/* @brief Height of the region in pixels. */
		uint32 mHeight = 0;

		/* @brief Depth of the region in pixels, used for Texture3D. */
		uint32 mDepth = 1;

		/* @brief Mip level this region targets. */
		uint32 mMipLevel = 0;

	};

	/* @brief Dimensionality and type of a GPU texture resource. */
	enum class TextureType : byte {
		None,
		Texture2D,
		Texture2DMS,
		Texture3D,
		Cubemap
	};

	/* @brief GPU-side internal storage format for a texture.
	* Determines how pixel data is stored and sampled on the GPU.
	*/
	enum class ImageLayout : byte {
		
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

		// Depth
		Depth16,
		Depth24,
		Depth32,
		Depth32F,

		// Depth/Stencil
		Depth24Stencil8,
		Depth32FStencil8,

		// Stencil
		StencilIndex8

	};

	/* @brief CPU-side pixel channel layout of the source texture data.
	* Describes how channels are ordered in the raw pixel buffer passed to the GPU.
	*/
	enum class ImageFormat : byte {
		RGBA,
		RGB,
		RG,
		R,
		BGRA,
		DepthComponent,
		DepthStencil
	};

	/* @brief Data type of each pixel channel in the CPU-side pixel buffer. */
	enum class TextureDataType : byte {
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
	struct FTextureDescriptor {

		/* @brief Raw pixel data loaded from the CPU side. */
		FTextureData mData;

		/* @brief GPU-side storage format for the texture. */
		ImageLayout mImageLayout = ImageLayout::RGBA8;

		/* @brief CPU-side pixel channel layout of the source data. */
		ImageFormat mImageFormat = ImageFormat::RGBA;

		/* @brief Data type of each pixel channel in the source buffer. */
		TextureDataType mDataType = TextureDataType::UnsignedByte;

		/* @brief Whether to automatically generate mipmaps after upload. */
		bool bGenerateMipmaps = false;

		/* @brief Number of mip levels to generate. 0 = calculate automatically. */
		uint32 mMipmapLevels = 0;

		/* @brief Number of samples for multisampled textures. 0 = not multisampled. */
		uint32 mSamples = 0;

		/* @brief Dimensionality and type of the texture. */
		TextureType mTextureType = TextureType::None;

		/* @brief Texture width in pixels. 0 = infer from mData. */
		uint32 mWidth = 0;

		/* @brief Texture height in pixels. 0 = infer from mData. */
		uint32 mHeight = 0;

		/* @brief Texture depth in pixels. Used for Texture3D only. */
		uint32 mDepth = 0;

		/* @brief Per-face pixel data for cubemap textures.
		* Faces are ordered: +X, -X, +Y, -Y, +Z, -Z.
		*/
		struct RCubemap {

			/* @brief Pixel data for each of the six cubemap faces. */
			FTextureData mFaces[6]{};

		} mCubemap;
	};

	/* @brief Describes a partial update to an existing GPU texture.
	* Used to upload new pixel data to a specific region without recreating the texture.
	*/
	struct FTextureUpdateDescriptor {

		/* @brief Target region within the texture to update. */
		RTextureRect mRect;

		/* @brief New pixel data to upload into the target region. */
		FTextureData mData;

		/* @brief Whether to regenerate mipmaps after the update. */
		bool bGenerateMipmaps = false;

	};

	/* @brief Internal GPU-side representation of an uploaded texture.
	* Stores the OpenGL handle alongside format and geometry metadata.
	*/
	struct FTexture {

		/* @brief Region and dimensions of the texture. */
		RTextureRect mRect;

		/* @brief Dimensionality and type of the texture. */
		TextureType mType = TextureType::None;

		/* @brief GPU-side internal storage format. */
		ImageLayout mInternalFormat = ImageLayout::RGBA8;

		/* @brief CPU-side pixel channel layout used during upload. */
		ImageFormat mDataFormat = ImageFormat::RGBA;

		/* @brief Data type of each pixel channel used during upload. */
		TextureDataType mDataType = TextureDataType::UnsignedByte;

		/* @brief Number of mip levels generated for this texture. */
		uint32 mMipmapLevels = 0;

		/* @brief Number of samples for multisampled textures. 0 = not multisampled. */
		uint32 mSamples = 0;

		/* @brief Underlying GPU texture handle (OpenGL object ID). */
		RTextureID mHandle = 0;

	};

} // namespace lum::rhi