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
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Vec4.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Utils/Assert.hpp"
#include "Core/Utils/SafePtr.hpp"

namespace lum {

	struct CTransform;
	struct CStaticMesh;
	struct CMaterialInstance;

	namespace rhi {
		class IRenderDevice;
	} // namespace rhi

	namespace ev {
		class EventBus;
	} // namespace ev

	class RendererContext {
	public:

		SafePtr<rhi::IRenderDevice> m_RenderDevice = nullptr;
		SafePtr<ev::EventBus> m_EventBus = nullptr;

		void Assert() const {
			LUM_ASSERT(m_RenderDevice, "RendererContext: IRenderDevice is nullptr");
		}

	};

	struct CameraData {

		Vector3 m_Position{};
		Vector3 m_Front{};
		Vector3 m_Up{};
		Vector3 m_Right{};
		float32 m_Fov = 60.0f; // In degrees

	};

	struct LUM_API RenderInstance {

		SafePtr<CTransform> m_Transform = nullptr;
		SafePtr<CStaticMesh> m_StaticMesh = nullptr;
		SafePtr<CMaterialInstance> m_Material = nullptr;

	};

} // namespace lum