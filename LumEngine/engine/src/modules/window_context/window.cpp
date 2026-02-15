#pragma once
#include "window_context/window.hpp"
#include "core/core_defines.hpp"
#include "core/utils/lum_assert.hpp"
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

	void OpenGLWindow::SetWidth(uint32 width) noexcept {
		mWidth = width;
	}
	void OpenGLWindow::SetHeight(uint32 height) noexcept {
		mHeight = height;
	}
	uint32 OpenGLWindow::GetWidth( ) const noexcept {
		return mWidth;
	}
	uint32 OpenGLWindow::GetHeight( ) const noexcept {
		return mHeight;
	}
	void* OpenGLWindow::GetNativeWindow( ) const noexcept {
		return mWindow;
	}
	bool OpenGLWindow::IsOpen( ) const noexcept {
		return !glfwWindowShouldClose(mWindow);
	}
	RenderBackend OpenGLWindow::GetBackend( ) const noexcept {
		return mBackend;
	}

	void OpenGLWindow::Init( const WindowDescriptor& desc ) {

		if (!glfwInit()) {
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#		if LUM_ENABLE_DEBUG_RENDER == 1
			glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_OPENGL_DEBUG_CONTEXT);
#		endif

		if (desc.bFullscreen) {
			mWindow = glfwCreateWindow(desc.mWidth, desc.mHeight, desc.mTitle, glfwGetPrimaryMonitor(), nullptr);
		}
		else mWindow = glfwCreateWindow(desc.mWidth, desc.mHeight, desc.mTitle, nullptr, nullptr);

		mWidth = desc.mWidth;
		mHeight = desc.mHeight;

		if (!mWindow) {
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(mWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			return;
		}

#		if LUM_ENABLE_IMGUI == 1
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
			ImGui_ImplOpenGL3_Init("#version 450");
#		endif
#		if LUM_ENABLE_DEBUG_RENDER == 1
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(rhi::detail::GLDebugCallback, nullptr);
#		endif

		mBackend = RenderBackend::OpenGL;

	}


	Window* CreateWindow( const WindowDescriptor& desc ) {
		switch (desc.mBackend) {
		case RenderBackend::OpenGL: return new OpenGLWindow(desc);
		}
		return nullptr;
	}

}