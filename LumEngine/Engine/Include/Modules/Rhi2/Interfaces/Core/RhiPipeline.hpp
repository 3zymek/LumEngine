#pragma once

#include "Rhi2/RhiCommon.hpp"
#include "Rhi2/Interfaces/Core/RhiShader.hpp"
#include "Core/Utils/HandlePool.hpp"

namespace lum::rhi {

	using PipelineID2 = uint32;
	struct PipelineHandle2 : public cstd::BaseHandle<PipelineID2> {};

	enum class PrimitiveTopology : uint8 {
		TriangleList = 0U,
		TriangleStrip,
		PointList,
		LineList,
		LineStrip,
	};

	struct AssemblyPass {

		PrimitiveTopology m_Topology = PrimitiveTopology::TriangleList;
		bool m_PrimitiveRestart = false;

	};

	enum class CullMode : uint8 {
		None = 0U,
		Front,
		Back,
	};

	enum class PolygonMode : uint8 {
		Fill = 0U,
		Wireframe,
	};

	enum class FrontFace : uint8 {
		CounterClockwise = 0U,
		Clockwise
	};

	struct RasterizationPass {

		bool m_RasterizerEnabled = true;

		FrontFace m_FrontFace = FrontFace::Clockwise;
		CullMode m_CullMode = CullMode::Back;
		PolygonMode m_PolygonMode = PolygonMode::Fill;
		float32 m_WireframeWidth = 1.0f;

		bool m_DepthBiasEnabled = false;
		float32 m_DepthBiasConstant = 0.0f;
		float32 m_DepthBiasSlope = 0.0f;
		float32 m_DepthBiasClamp = 0.0f;


	};

	enum class BlendFactor : uint8 {
		Zero = 0U,
		One,

		SrcColor,
		OneMinusSrcColor,

		DstColor,
		OneMinusDstColor,

		SrcAlpha,
		OneMinusSrcAlpha,

		DstAlpha,
		OneMinusDstAlpha,

		ConstantColor,
		OneMinusConstantColor,

		ConstantAlpha,
		OneMinusConstantAlpha,
	};

	enum class BlendOp : uint8 {
		Add = 0U,
		Substract,
		ReverseSubstract,
		Min,
		Max
	};

	enum class ColorComponentFlag : bitfield {
		None = 1 << 0,
		Red = 1 << 1,
		Green = 1 << 2,
		Blue = 1 << 3,
		Alpha = 1 << 4,

		RGB = Red | Green | Blue,
		RGBA = Red | Green | Blue | Alpha,

	};

} // namespace lum::rhi

namespace lum {

	LUM_ENABLE_ENUM_BITFLAG_OPERATIONS(rhi::ColorComponentFlag);

} // namespace lum

namespace lum::rhi {

	struct ColorBlendAttachment {

		bool m_BlendEnabled = false;
		BlendFactor m_SrcColorBlendFactor{};
		BlendFactor m_DstColorBlendFactor{};
		BlendFactor m_SrcAlphaBlendFactor{};
		BlendFactor m_DstAlphaBlendFactor{};

		BlendOp m_ColorBlendOp{};
		BlendOp m_AlphaBlendOp{};

		Flags<ColorComponentFlag> m_ColorMask =
			ColorComponentFlag::Red | ColorComponentFlag::Green | ColorComponentFlag::Blue | ColorComponentFlag::Alpha;

	};

	struct ColorBlendPass {

		std::vector<ColorBlendAttachment> m_Attachments{};
		Vector4 m_BlendConstants{};

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

	struct MultisamplePass {

		SampleCount m_SampleCount = SampleCount::Sample1;

		bool m_AlphaToCoverage = false;
		bool m_AlphaToOne = false;

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

	struct VertexAttribute {

		uint32 m_Location = 0;
		uint32 m_Binding = 0;
		uint32 m_Offset = 0;

		ImageFormat m_Format = ImageFormat::RGBA8_UNORM;

	};

	struct VertexBinding {

		uint32 m_Binding = 0;
		uint32 m_Stride = 0;
		bool m_Instanced = false;

	};

	struct VertexInputPass {

		std::vector<VertexAttribute> m_Attributes{};
		std::vector<VertexBinding> m_Bindings{};

	};

	struct PipelineCreateInfo2 {

		std::vector<ShaderInfo2> m_ShaderInfos{};
		AssemblyPass		m_AssemblyPass{};
		RasterizationPass	m_RasterizationPass{};
		ColorBlendPass		m_ColorBlendPass{};
		MultisamplePass		m_MultisamplePass{};
		VertexInputPass		m_VertexInputPass{};
		

	};


} // namespace lum::rhi