//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Texture configuration structures (descriptors, formats, types)
//
//=============================================================================//
#pragma once
#include "Rhi/RhiCommon.hpp"

namespace lum::rhi {

	/* @brief Defines a rectangular region within a texture, used for partial updates.
	* Also carries mip level and depth slice for 3D or array texture operations.
	*/
	struct TextureRect {

		/* @brief X offset in pixels from the left edge of the texture. */
		uint32 m_X = 0;

		/* @brief Y offset in pixels from the top edge of the texture. */
		uint32 m_Y = 0;

		/* @brief Width of the region in pixels. */
		uint32 m_Width = 0;

		/* @brief Height of the region in pixels. */
		uint32 m_Height = 0;

		/* @brief Depth of the region in pixels, used for Texture3D. */
		uint32 m_Depth = 1;

		/* @brief Mip level this region targets. */
		uint32 m_MipLevel = 0;

	};

	/* @brief Dimensionality and type of a GPU texture resource. */
	enum class TextureKind {
		None,
		Texture2D,				/* Standard 2D texture (render target, GBuffer attachment). */
		Texture2DArray,			/* Array with multiple texture 2D layers. */
		Texture2DMultiSampled,	/* 2D multisample texture (MSAA render target). */
		Image3D,				/* Volumetric 3D texture. */
		Cubemap					/* 6-face cube texture (skybox, IBL environment maps). */
	};

	/* @brief GPU-side internal storage format for a texture.
	* Determines how pixel data is stored and sampled on the GPU.
	*/
	enum class TextureFormat {
		
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
	enum class PixelLayout {
		RGBA,
		RGB,
		RG,
		R,
		BGRA,
		DepthComponent,
		DepthStencil
	};

	/* @brief Data type of each pixel channel in the CPU-side pixel buffer. */
	enum class PixelDataType {
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
	* can be omitted if they are already present in m_Data, in which case they
	* will be inferred automatically.
	*
	*/
	struct TextureCreateInfo {

		/* @brief Raw pixel data loaded from the CPU side. */
		ImageData m_Data{};

		/* @brief GPU-side storage format for the texture. */
		TextureFormat m_InternalFormat = TextureFormat::RGBA8;

		/* @brief CPU-side pixel channel layout of the source data. */
		PixelLayout m_PixelFormat = PixelLayout::RGBA;

		/* @brief Data type of each pixel channel in the source buffer. */
		PixelDataType m_DataType = PixelDataType::UnsignedByte;

		/* @brief Whether to automatically generate mipmaps after upload. */
		bool m_GenerateMipmaps = false;

		/* @brief Number of mip levels to generate. 0 = calculate automatically. */
		uint32 m_MipmapLevels = 0;

		/* @brief Number of samples for multisampled textures. 0 = not multisampled. */
		uint32 m_Samples = 0;

		/* @brief Dimensionality and type of the texture. */
		TextureKind m_TextureType = TextureKind::None;

		/* @brief Texture width in pixels. 0 = infer from m_Data. */
		uint32 m_Width = 0;

		/* @brief Texture height in pixels. 0 = infer from m_Data. */
		uint32 m_Height = 0;

		/* @brief Texture depth in pixels. Used for Texture3D only. */
		uint32 m_Depth = 0;

		/* @brief Per-face pixel data for cubemap textures.
		* Faces are ordered: +X, -X, +Y, -Y, +Z, -Z.
		*/
		struct Cubemap {

			/* @brief Pixel data for each of the six cubemap faces. */
			ImageData m_Faces[6]{};

		} m_Cubemap{};
	};

	/* @brief Describes a partial update to an existing GPU texture.
	* Used to upload new pixel data to a specific region without recreating the texture.
	*/
	struct TextureUpdateDescription {

		/* @brief Target region within the texture to update. */
		TextureRect m_Rect{};

		/* @brief New pixel data to upload into the target region. */
		ImageData m_Data{};

		/* @brief Whether to regenerate mipmaps after the update. */
		bool m_GenerateMipmaps = false;

	};

	/* @brief Internal GPU-side representation of an uploaded texture.
	* Stores the OpenGL handle alongside format and geometry metadata.
	*/
	struct Texture {

		/* @brief Region and dimensions of the texture. */
		TextureRect m_Rect{};

		/* @brief Dimensionality and type of the texture. */
		TextureKind m_Kind = TextureKind::None;

		/* @brief GPU-side internal storage format. */
		TextureFormat m_Format = TextureFormat::RGBA8;

		/* @brief CPU-side pixel channel layout used during upload. */
		PixelLayout m_PixelLayout = PixelLayout::RGBA;

		/* @brief Data type of each pixel channel used during upload. */
		PixelDataType m_PixelDataType = PixelDataType::UnsignedByte;

		/* @brief Number of mip levels generated for this texture. */
		uint32 m_MipmapLevels = 0;

		/* @brief Number of samples for multisampled textures. 0 = not multisampled. */
		uint32 m_SampleCount = 0;

		/* @brief Underlying GPU texture handle. */
		TextureID m_Handle = 0;

	};

} // namespace lum::rhi