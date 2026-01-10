#pragma once
#include "window_context/window.hpp"
#include "core/core_defines.hpp"
namespace lum {

	namespace rhi::detail {
#if defined(DEBUG_RENDER)
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

			if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // disable unnecessary spam 
			if (severity == GL_DEBUG_SEVERITY_LOW)
				LOG_INFO(msg);
			if (severity == GL_DEBUG_SEVERITY_MEDIUM)
				LOG_WARN(msg);
			if (severity == GL_DEBUG_SEVERITY_HIGH)
				LOG_FATAL(msg);

		}
#endif
	}

	///  OpenGL window
	unsigned int OpenGL_Window::GetWidth( ) {
		return width;
	}
	unsigned int OpenGL_Window::GetHeight( ) {
		return height;
	}
	void* OpenGL_Window::GetNativeWindow( ) {
		return m_window;
	}
	bool OpenGL_Window::IsOpen( ) {
		return !glfwWindowShouldClose(m_window);
	}
	RenderBackend OpenGL_Window::GetBackend( ) {
		return backend;
	}

	void OpenGL_Window::Init( const WindowDescriptor& desc ) {

		if (!glfwInit()) {
			LOG_FATAL("Failed to initialize GLFW");
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(DEBUG_RENDER)
		glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_OPENGL_DEBUG_CONTEXT);
#endif
		if (desc.MSAA_samples > 0)
			glfwWindowHint(GLFW_SAMPLES, desc.MSAA_samples);

		if (desc.fullscreen) {
			m_window = glfwCreateWindow(desc.width, desc.height, desc.title, glfwGetPrimaryMonitor(), nullptr);
		}
		else m_window = glfwCreateWindow(desc.width, desc.height, desc.title, nullptr, nullptr);

		if (!m_window) {
			LOG_FATAL("Failed to initialize window");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG_FATAL("Failed to initialize GLAD");
			glfwTerminate();
			return;
		}

#if defined(DEBUG_RENDER)
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(rhi::detail::GLDebugCallback, nullptr);
#endif

		backend = RenderBackend::OpenGL;

	}


	Window* CreateWindow( const WindowDescriptor& desc ) {
		switch (desc.render) {
		case RenderBackend::OpenGL: return new OpenGL_Window(desc);
		}
		return nullptr;
	}

}