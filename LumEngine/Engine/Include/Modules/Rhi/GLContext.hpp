#pragma once

#include <glad/glad.h>

namespace lum {

	class RenderContext {
	public:
		virtual ~RenderContext( ) = default;
		virtual void SwapBuffers( ) = 0;
	};

	class GLContext : public RenderContext {
	public:

		virtual ~GLContext( ) = default;

		virtual void MakeCurrent( ) = 0;
		virtual bool LoadGLFunctions(  ) = 0;
		virtual void SwapBuffers( ) = 0;

	};

}