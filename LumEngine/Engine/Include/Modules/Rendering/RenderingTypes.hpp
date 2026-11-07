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
#include "Core/Utils/Assert.hpp"
#include "Core/Utils/SafePtr.hpp"

namespace lum {

	struct CTransform;
	struct CStaticMesh;
	struct CMaterialInstance;

	namespace rhi {
		class IRenderDevice;
	} // namespace rhi

	class RendererContext {
	public:

		SafePtr<rhi::IRenderDevice> m_RenderDevice = nullptr;

		void Assert( ) const {
			LUM_ASSERT( m_RenderDevice, "RendererContext: IRenderDevice is nullptr" );
		}

	};

	struct LUM_API RenderInstance {

		SafePtr<CTransform> m_Transform = nullptr;
		SafePtr<CStaticMesh> m_StaticMesh = nullptr;
		SafePtr<CMaterialInstance> m_Material = nullptr;

	};

} // namespace lum