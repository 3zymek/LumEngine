//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Static mesh component for the ECS.
//
//=============================================================================//
#pragma once

#include "render/mesh_manager.hpp"
#include "entity/ecs_common.hpp"

namespace lum {

	/* @brief ECS component holding a handle to a static mesh asset.
	*  Does not own the mesh data — lifetime is managed by MMeshManager.
	*  Use StaticMeshHandle to reference geometry uploaded to the GPU.
	*/
	struct CStaticMesh : public Component {

		/* @brief Handle to the static mesh asset registered in MMeshManager. */
		StaticMeshHandle mMesh;

	};

} // namespace lum