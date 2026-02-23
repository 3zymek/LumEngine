#pragma once

#include "render/mesh_manager.hpp"
#include "entity/ecs_common.hpp"

namespace lum {

	struct CStaticMesh : Component {

		StaticMeshHandle mMesh;

	};

}