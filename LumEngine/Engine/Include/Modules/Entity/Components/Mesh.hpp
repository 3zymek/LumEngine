/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once
#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Utils/TypeID.hpp"
#include "Entity/EcsTypes.hpp"

#include "Entity/Components/Mesh.gen.hpp"

namespace lum {

	/* @brief ECS component holding a handle to a static mesh asset.
	*  Does not own the mesh data — lifetime is managed by MeshManager.
	*  Use StaticMeshHandle to reference geometry uploaded to the GPU.
	*/
	LUM_CLASS( Category = "RENDERING" ) 
	struct CStaticMesh : public ComponentBase {

		LUM_GENERATED_BODY( )

		LUM_PROPERTY( Droppable = "FILE" )
		String m_Path{};

		String m_LastLoadedPath = "";

		/* @brief Handle to the static mesh asset registered in MeshManager. */
		//StaticMeshHandle m_Handle;

	};

	LUM_CLASS_EXTENSIONS( )

} // namespace lum