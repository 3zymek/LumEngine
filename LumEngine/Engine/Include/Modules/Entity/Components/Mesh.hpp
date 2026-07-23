//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Static mesh component for the ECS.
//
//=============================================================================//
#pragma once
#include "Entity/EcsCommon.hpp"
#include "Render/RenderCommon.hpp"
#include "Entity/Components/Mesh.gen.hpp"

namespace lum {

	/* @brief ECS component holding a handle to a static mesh asset.
	*  Does not own the mesh data — lifetime is managed by MMeshManager.
	*  Use StaticMeshHandle to reference geometry uploaded to the GPU.
	*/
	LCLASS( Category = "RENDERING" ) 
	struct CStaticMesh : public ComponentBase {

		LUM_GENERATED_BODY( )

		LPROPERTY( Droppable = "FILE" )
		String mPath;

		String mLastLoadedPath = "";

		/* @brief Handle to the static mesh asset registered in MMeshManager. */
		StaticMeshHandle mHandle;

	};

	LUM_CLASS_EXTENSIONS( )

} // namespace lum