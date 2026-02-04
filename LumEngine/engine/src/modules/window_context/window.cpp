#pragma once
#include "window_context/window.hpp"
#include "core/core_defines.hpp"
#include "core/lum_assert.hpp"
#if LUM_ENABLE_IMGUI == 1
	#include "imgui.h"
	#include "imgui_impl_glfw.h"
	#include "imgui_impl_opengl3.h"
#endif
namespace lum {

	namespace rhi::detail {
#		if LUM_ENABLE_DEBUG_RENDER == 1
		void APIENTRY GLDebugCallback(
			GLenum src,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const char* msg,
			const void* usrParam
		)
		{
			if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
				LUM_LOG_DEBUG(msg);
			if (severity == GL_DEBUG_SEVERITY_LOW);
				LUM_LOG_INFO(msg);
			if (severity == GL_DEBUG_SEVERITY_MEDIUM);
				LUM_LOG_WARN(msg);
			if (severity == GL_DEBUG_SEVERITY_HIGH);
				LUM_LOG_ERROR(msg);

		}
#		endif
	}

	///  OpenGL window
	uint32 OpenGL_Window::GetWidth( ) {
		return width;
	}
	uint32 OpenGL_Window::GetHeight( ) {
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
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#		if LUM_ENABLE_DEBUG_RENDER == 1
			glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_OPENGL_DEBUG_CONTEXT);
#		endif
		if (desc.MSAA_samples > 0)
			glfwWindowHint(GLFW_SAMPLES, desc.MSAA_samples);

		if (desc.fullscreen) {
			m_window = glfwCreateWindow(desc.width, desc.height, desc.title, glfwGetPrimaryMonitor(), nullptr);
		}
		else m_window = glfwCreateWindow(desc.width, desc.height, desc.title, nullptr, nullptr);

		width = desc.width;
		height = desc.height;

		if (!m_window) {
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			return;
		}

#		if LUM_ENABLE_IMGUI == 1
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGui_ImplGlfw_InitForOpenGL(m_window, true);
			ImGui_ImplOpenGL3_Init("#version 450");
#		endif
#		if LUM_ENABLE_DEBUG_RENDER == 1
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(rhi::detail::GLDebugCallback, nullptr);
#		endif

		backend = RenderBackend::OpenGL;

	}


	Window* CreateWindow( const WindowDescriptor& desc ) {
		switch (desc.render) {
		case RenderBackend::OpenGL: return new OpenGL_Window(desc);
		}
		return nullptr;
	}

}