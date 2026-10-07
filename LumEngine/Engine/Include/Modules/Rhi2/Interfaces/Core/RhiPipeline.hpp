#pragma once

#include "Rhi2/RhiCommon.hpp"
#include "Rhi2/Interfaces/Core/RhiShader.hpp"
#include "Core/Utils/HandlePool.hpp"

namespace lum::rhi {

	using PipelineID2 = uint32;
	struct PipelineHandle2 : public cstd::BaseHandle<PipelineID2>{};

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

	struct PipelineCreateInfo2 {

		std::vector<ShaderInfo2> m_ShaderInfos{};
		AssemblyPass m_AssemblyPass{};
		RasterizationPass m_RasterizationPass{};

	};


} // namespace lum::rhi