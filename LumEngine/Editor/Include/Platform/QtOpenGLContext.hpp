#pragma once

#include "Platform/OpenGLContext.hpp"
#include "Core/Utils/SafePtr.hpp"

class QOpenGLContext;
class QSurface;

namespace lum::editor {

	class QtOpenGLContext : public OpenGLContext {
	public:

		QtOpenGLContext( QOpenGLContext& context, QSurface& surface) {}

		void MakeCurrent( ) override;
		bool Initialize( ) override;
		void SwapBuffers( ) override;
		

	private:

		SafePtr<QOpenGLContext> mContext = nullptr;
		SafePtr<QSurface> mSurface = nullptr;

	};

}