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
		None	= 1 << 0,
		Red		= 1 << 1,
		Green	= 1 << 2,
		Blue	= 1 << 3,
		Alpha	= 1 << 4
	};

} // namespace lum::rhi

namespace lum {

	LUM_ENABLE_ENUM_BITFLAG_OPERATIONS( rhi::ColorComponentFlag );

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

	struct PipelineCreateInfo2 {

		std::vector<ShaderInfo2> m_ShaderInfos{};
		AssemblyPass		m_AssemblyPass{};
		RasterizationPass	m_RasterizationPass{};
		ColorBlendPass		m_ColorBlendPass{};

	};


} // namespace lum::rhi