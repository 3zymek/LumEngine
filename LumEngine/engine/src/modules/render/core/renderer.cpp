#include "render/core/renderer.hpp"
#include "core/core_pch.hpp"
namespace lum {
	namespace render {

		void APIENTRY GLDebugCallback(
			GLenum src,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* msg,
			const void* usrParam
		)
		{

			if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
			LOG_DEBUG(msg);

		}

		void Renderer::Init(RenderConfig params) {

			if (!glfwInit()) {
				LOG_ERROR("failed to initialize GLFW");
				return;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_SAMPLES, params.MSAA_samples);

			#ifdef DEBUG_RENDER
				glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_OPENGL_DEBUG_CONTEXT);
			#endif

			const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			m_window_width = video_mode->width;
			m_window_height = video_mode->height;

			if (params.fullscreen) {
				m_window = GLFWWindowPtr(glfwCreateWindow(
					m_window_width,
					m_window_height,
					params.title.c_str(),
					glfwGetPrimaryMonitor(),
					nullptr)
				);
			}
			else
				m_window = GLFWWindowPtr(glfwCreateWindow(
					params.width,
					params.height,
					params.title.c_str(),
					nullptr,
					nullptr)
				);

			if (!m_window) {
				LOG_ERROR("failed to create window");
				glfwTerminate();
				return;
			}

			glfwMakeContextCurrent(m_window.get());

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				LOG_ERROR("failed to init GLAD");
				glfwTerminate();
				return;
			}

			#ifdef DEBUG_RENDER
				glDebugMessageCallback(GLDebugCallback, nullptr);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glEnable(GL_DEBUG_OUTPUT);
			#endif

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_MULTISAMPLE);

		}

		void Renderer::BeginFrame() {

			glViewport(0, 0, m_window_width, m_window_height);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwPollEvents();

		}

		void Renderer::EndFrame() {

			glfwSwapBuffers(m_window.get());

		}

		bool Renderer::WindowIsOpen() {
			return !glfwWindowShouldClose(m_window.get());
		}
	}

}