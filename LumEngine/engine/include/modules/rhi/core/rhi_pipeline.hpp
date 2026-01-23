#pragma once
#include "rhi/rhi_common.hpp"

namespace lum::rhi {

	struct PipelineHandle {

		pipelineid id = max_val<pipelineid>();
		pipelineid generation = 0;

	};

	struct PipelineDescriptor {

		PolygonMode polygon_mode = PolygonMode::fill;
		Face polygon_mode_faces = Face::front_back;

	};

	struct Pipeline {

		PolygonMode polygon_mode{};
		Face polygon_mode_faces{};

	};

}