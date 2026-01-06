#pragma once
#include "entity/ecs_common.hpp"
#include "render/core/mesh_storage.hpp"
namespace lum::ecs::components {
	struct MeshComponent {

		render::MeshHandle handle;

		LumComponentTag;

	};
}
