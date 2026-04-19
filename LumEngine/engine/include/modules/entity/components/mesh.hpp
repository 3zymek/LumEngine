//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Static mesh component for the ECS.
//
//=============================================================================//
#pragma once
#include "entity/ecs_common.hpp"
#include "render/render_common.hpp"

namespace lum {

	/* @brief ECS component holding a handle to a static mesh asset.
	*  Does not own the mesh data — lifetime is managed by MMeshManager.
	*  Use StaticMeshHandle to reference geometry uploaded to the GPU.
	*/
	LCLASS( Category = "Rendering" ) struct CStaticMesh : public Component {

		LPROPERTY( )
		String mPath;

		/* @brief Handle to the static mesh asset registered in MMeshManager. */
		StaticMeshHandle mHandle;

	};

} // namespace lum