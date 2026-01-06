#pragma once
#include "render/render_pch.hpp"
#include "core/core_defines.hpp"
#include "render/render_common.hpp"
namespace lum {
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

		class RenderDevice {

			using GLFWWindowPtr = std::unique_ptr<GLFWwindow, detail::GLFWWinDestructor>;

		public:

			RenderDevice( ) {}

			void Init			( RenderConfig );
			void BeginFrame		( );
			void EndFrame		( );
			bool WindowIsOpen	( );

			inline double GetAspectRatio( ) { return to_d(m_window_width) / to_d(m_window_height); }
			inline GLFWwindow* GetWindow( ) { return m_window.get(); }

		private:

			unsigned int	m_window_width = 0, m_window_height = 0;
			GLFWWindowPtr	m_window = nullptr;

		};
	}
}