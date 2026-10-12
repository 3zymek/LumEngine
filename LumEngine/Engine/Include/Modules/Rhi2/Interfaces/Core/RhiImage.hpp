#pragma once
#include "Rhi2/RhiCommon.hpp"
#include "Core/Utils/HandlePool.hpp"
#include "Core/Utils/ResourceLoader.hpp"

namespace lum::rhi {

	using ImageID2 = uint32;
	struct ImageHandle2 : cstd::BaseHandle<ImageID2> {};

	enum class ImageUsageFlag : bitfield {

		None = 0,

		TransferSource = 1 << 0,
		TransferDestination = 1 << 1,

		ShaderReadable = 1 << 2, // Sampled
		ShaderReadWrite = 1 < 3, // Storage

		ColorAttachment = 1 << 4,
		DepthStencilAttachment = 1 << 5,

		InputAttachment = 1 << 6,

		TransientAttachment = 1 << 7

	};
	using ImageUsageFlags = Flags<ImageUsageFlag>;

	enum class ImageType : uint8 {
		Image1D = 0U,
		Image2D,
		Image3D,
	};

	enum class ImageFormat : uint8 {

		R8_UNORM = 0U,
		R8_SNORM,
		R8_UINT,
		R8_SINT,
		R8_SRGB,

		RG8_UNORM,
		RG8_SNORM,
		RG8_UINT,
		RG8_SINT,
		RG8_SRGB,

		RGB8_UNORM,
		RGB8_SNORM,
		RGB8_UINT,
		RGB8_SINT,
		RGB8_SRGB,

		RGBA8_UNORM,
		RGBA8_SNORM,
		RGBA8_UINT,
		RGBA8_SINT,
		RGBA8_SRGB,


		R16_UNORM,
		R16_SNORM,
		R16_UINT,
		R16_SINT,
		R16_FLOAT,

		RG16_UNORM,
		RG16_SNORM,
		RG16_UINT,
		RG16_SINT,
		RG16_FLOAT,

		RGB16_UNORM,
		RGB16_SNORM,
		RGB16_UINT,
		RGB16_SINT,
		RGB16_FLOAT,

		RGBA16_UNORM,
		RGBA16_SNORM,
		RGBA16_UINT,
		RGBA16_SINT,
		RGBA16_FLOAT,


		R32_UINT,
		R32_SINT,
		R32_FLOAT,

		RG32_UINT,
		RG32_SINT,
		RG32_FLOAT,

		RGB32_UINT,
		RGB32_SINT,
		RGB32_FLOAT,

		RGBA32_UINT,
		RGBA32_SINT,
		RGBA32_FLOAT

	};

	struct ImageExtent3D {

		uint32 m_Width = 0;
		uint32 m_Height = 0;
		uint32 m_Depth = 0;

	};

	enum class SampleCount : uint8 {
		Sample1 = 0U,
		Sample2,
		Sample4,
		Sample8,
		Sample16,
		Sample32,
		Sample64
	};

	struct ImageCreateInfo2 {

		ImageUsageFlags m_UsageFlags = ImageUsageFlag::None;
		ImageType m_Type = ImageType::Image2D;
		ImageFormat m_Format = ImageFormat::RGBA32_FLOAT;
		ImageExtent3D m_Extent{};
		SampleCount m_SampleCount = SampleCount::Sample1;

		uint32 m_MipmapLevels = 1;
		uint32 m_ArrayLayers = 1;

		ImageData m_Data{};

	};

	struct Image2 {

	};

} // namespace lum::rhi