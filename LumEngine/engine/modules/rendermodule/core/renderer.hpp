#pragma once
#include "glad/glad.h"
#include "glfw3.h"
#include "core_defines.hpp"
#include "rendermodule/details/render_define.hpp"
#include <memory>
namespace render {

	namespace detail {
		struct GLFWWinDestructor {
			void operator()(GLFWwindow* window) {
				if (window) glfwDestroyWindow(window);
			}
		};
		struct GLFWVidDestructor {
			void operator()(GLFWvidmode* mode) {
				if (mode) mode = nullptr;
			}
		};
	}

	class Renderer {

		using GLFWWindowPtr = std::unique_ptr<GLFWwindow, detail::GLFWWinDestructor>;

	public:

		Renderer() {}

		void Init		( RenderInitParams );
		void StartFrame	( );
		void EndFrame	( );
		bool WindowIsOpen();

	private:

		unsigned int m_window_width = 0, m_window_height = 0;
		GLFWWindowPtr	m_window = nullptr;

	};
}