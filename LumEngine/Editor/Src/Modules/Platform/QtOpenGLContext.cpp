//========= Copyright (C) 2025-present 3zymek, MIT License  ============//
//
// Purpose: Qt offscreen surface OpenGL rendering context implementation.
//
//=============================================================================//

#include "Platform/QtOpenGLContext.hpp"
#include "Core/Utils/Logger.hpp"
#include "Core/Utils/LumAssert.hpp"

namespace lum::editor {

	void QtOpenGLContext::Initialize( ) {

		QSurfaceFormat format{};
		format.setRenderableType( QSurfaceFormat::RenderableType::OpenGL );
		format.setProfile( QSurfaceFormat::CoreProfile );
		format.setVersion( 4, 5 );

		m_Context = new QOpenGLContext( );
		m_Context->setFormat( format );

		if (!m_Context->create( )) {
			LUM_LOG_FATAL( "Failed to create QOpenGLContext" );
			return;
		}

		m_Surface = new QOffscreenSurface( );
		m_Surface->setFormat( m_Context->format( ) );
		m_Surface->create( );

		if (!m_Surface->isValid( )) {
			LUM_LOG_FATAL( "Failed to create QOffscreenSurface" );
			return;
		}

	}

	void QtOpenGLContext::SwapBuffers( ) {
		m_Context->swapBuffers( m_Surface );
	}

	void QtOpenGLContext::MakeCurrent( ) {
		m_Context->makeCurrent( m_Surface );
	}

	void* QtOpenGLContext::GetProcAddress( const char* functionName ) {

		return reinterpret_cast<void*>(m_Context->getProcAddress( functionName ));

	}

}