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
#include "Core/CorePch.hpp"

// Header impl file
#include "RenderModule.hpp"

#include "Event/EventBus.hpp"
#include "PlatformModule.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/RenderingTypes.hpp"
#include "ResModule.hpp"

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>

namespace lum {

	void RenderModule::Initialize(const RenderModuleCreateInfo& info) {

		RendererContext ctx{};
		ctx.m_RenderDevice = info.m_RenderDevice;
		ctx.m_EventBus = info.m_EventBus;

		m_Renderer.Initialize(ctx, info.m_EventBus.Ref());
		//m_RenderSys.Initialize( &m_Renderer );

		Slang::ComPtr<slang::IGlobalSession> globalSession;

		const SlangResult result =
			slang::createGlobalSession(globalSession.writeRef());

		if (result != SLANG_OK) {
			std::cout << "Slang initialization failed!\n";
			return;
		}

		std::cout << "Slang initialized successfully!\n";

	}

} // namespace lum