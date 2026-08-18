#pragma once

#include "Rhi/GLContext.hpp"
#include <glad/glad.h>
#include <glfw3.h>

namespace lum {

	class GLFWContext : public GLContext {
	public:

		GLFWContext( GLFWwindow& window ) : mWindow( &window ) {}

		void MakeCurrent( ) override {
			glfwMakeContextCurrent( mWindow );
		}

		bool LoadGLFunctions( ) override {
			return gladLoadGLLoader(
				reinterpret_cast<GLADloadproc>(glfwGetProcAddress)
			);
		}

		void SwapBuffers( ) override {
			glfwSwapBuffers( mWindow );
		}

	private:

		GLFWwindow* mWindow = nullptr;

	};

}