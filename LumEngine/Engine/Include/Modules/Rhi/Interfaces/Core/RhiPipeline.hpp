#pragma once

#include <vector>

#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Math/Vec4.hpp"
#include "Core/Utils/HandlePool.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiShader.hpp"

namespace lum::rhi {

	using PipelineID = uint32;
	struct PipelineHandle : public BaseHandle<PipelineID> {};

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
	using ColorComponentFlags = Flags<ColorComponentFlag>;

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

		ColorComponentFlags m_ColorMask =
			ColorComponentFlag::Red | ColorComponentFlag::Green | ColorComponentFlag::Blue | ColorComponentFlag::Alpha;

	};

	struct ColorBlendPass {

		std::vector<ColorBlendAttachment> m_Attachments{};
		Vector4 m_BlendConstants{};

	};

	struct MultisamplePass {

		SampleCount m_SampleCount = SampleCount::Sample1;

		bool m_AlphaToCoverage = false;
		bool m_AlphaToOne = false;

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

	class VertexLayoutConfiguration {
	public:

		LUM_NODISCARD static const VertexLayoutConfiguration& GetStatic() noexcept {
			static VertexLayoutConfiguration val{};
			return val;
		}

		LUM_NODISCARD VertexInputPass GetVertexInputPass() const {

			VertexInputPass info{};

			VertexBinding binding{};
			binding.m_Binding = 0;
			binding.m_Instanced = false;
			binding.m_Stride = sizeof(Vertex);
			info.m_Bindings.push_back(binding);

			info.m_Attributes.reserve(3);

			info.m_Attributes.push_back({ 0, 0, 0, ImageFormat::RGB32_FLOAT });
			info.m_Attributes.push_back({ 1, 0, 12, ImageFormat::RGB32_FLOAT });
			info.m_Attributes.push_back({ 2, 0, 24, ImageFormat::RG32_FLOAT });

			return info;
		}

	private:

		VertexLayoutConfiguration(const VertexLayoutConfiguration&) = delete;
		VertexLayoutConfiguration(VertexLayoutConfiguration&&) = delete;
		VertexLayoutConfiguration() = default;

	};

	struct PipelineCreateInfo {

		std::vector<ShaderInfo> m_ShaderInfos{};
		AssemblyPass		m_AssemblyPass{};
		RasterizationPass	m_RasterizationPass{};
		ColorBlendPass		m_ColorBlendPass{};
		MultisamplePass		m_MultisamplePass{};
		VertexInputPass		m_VertexInputPass{};

	};


} // namespace lum::rhi