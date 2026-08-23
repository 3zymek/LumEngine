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

		mContext = new QOpenGLContext( );
		mContext->setFormat( format );

		if (!mContext->create( )) {
			LUM_LOG_FATAL( "Failed to create QOpenGLContext" );
			return;
		}

		mSurface = new QOffscreenSurface( );
		mSurface->setFormat( mContext->format( ) );
		mSurface->create( );

		if (!mSurface->isValid( )) {
			LUM_LOG_FATAL( "Failed to create QOffscreenSurface" );
			return;
		}

	}

	void QtOpenGLContext::SwapBuffers( ) {
		mContext->swapBuffers( mSurface );
	}

	void QtOpenGLContext::MakeCurrent( ) {
		mContext->makeCurrent( mSurface );
	}

	void* QtOpenGLContext::GetProcAddress( const char* functionName ) {

		return reinterpret_cast<void*>(mContext->getProcAddress( functionName ));

	}

}