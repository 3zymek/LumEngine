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

		_Count // PrimitiveTopology enums count
	};

	struct AssemblyPass {

		PrimitiveTopology m_Topology = PrimitiveTopology::TriangleList;
		bool m_PrimitiveRestart = false;

	};

	enum class CullMode : uint8 {
		None = 0U,
		Front,
		Back,

		_Count // CullMode enums count
	};

	enum class PolygonMode : uint8 {
		Fill = 0U,
		Wireframe,

		_Count // PolygonMode enums count
	};

	struct RasterizationPass {

		CullMode m_CullMode = CullMode::Back;
		PolygonMode m_PolygonMode = PolygonMode::Fill;
		uint32 m_WireframeWidth = 1.0f;

	};

	struct PipelineCreateInfo2 {

		std::vector<ShaderInfo2> m_ShaderInfos{};
		AssemblyPass m_AssemblyPass{};

	};


} // namespace lum::rhi