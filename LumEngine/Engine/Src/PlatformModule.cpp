//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform module — window and render device lifecycle.
//
//=============================================================================//

#include "PlatformModule.hpp"
#include "Platform/Input/Input.hpp"
#include "Rhi/Core/RhiDevice.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void PlatformModule::Initialize( EngineCreateInfo info, ev::EventBus& bus ) {

		info.m_Window.m_Title = info.m_ApplicationName;

		if (!info.m_RenderContext) {

			// Create default window with default LumEngine backends

			m_Window = Window{};
			m_Window.Value( ).Initialize( info.m_Window, bus );

			auto* glfwWindow = static_cast<GLFWwindow*>(m_Window.Value( ).GetNativeWindow( ));

			m_DefaultRenderContext = OwningPtr<GLFWOpenGLContext>::Create( glfwWindow );
			m_ActiveRenderContext = m_DefaultRenderContext;

		}
		else {
			m_ActiveRenderContext = info.m_RenderContext;
		}

		m_RenderDevice = rhi::CreateDevice( info.m_RenderingBackend );
		m_RenderDevice( ).Initialize( m_ActiveRenderContext( ) );

	}

	void PlatformModule::Finalize( ) {
		m_RenderDevice( ).Finalize( );
	}

} // namespace lum