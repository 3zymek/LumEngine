#include "Platform/QtOpenGLContext.hpp"
#include "EditorPch.hpp"

namespace lum::editor {

	void QtOpenGLContext::MakeCurrent( ) {
		mContext( ).makeCurrent( &mSurface( ) );
	}

	bool QtOpenGLContext::Initialize( ) {
		
		static SafePtr<QOpenGLContext> sContext = nullptr;
		sContext = mContext;

		const bool result = gladLoadGLLoader(
			[]( const char* name ) {
				return reinterpret_cast<void*>(
					sContext( ).getProcAddress( name )
				);
			}
		);

		sContext = nullptr;
		return result;
	}

	void QtOpenGLContext::SwapBuffers( ) {
		mContext( ).swapBuffers( &mSurface( ) );
	}
	
}