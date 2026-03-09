//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL device state core management and rendering commands
//          Viewport, color/depth/stencil clearing, draw calls,
//          frame begin/end, and state queries
//
//=============================================================================//

#include "rhi/backend/opengl/gl_device.hpp"
#include "rhi/core/rhi_device.hpp"
#include "rhi/rhi_common.hpp"

#include "platform/window.hpp"

#if LUM_ENABLE_IMGUI == 1
	#include <imgui.h>
	#include <imgui_impl_glfw.h>
	#include <imgui_impl_opengl3.h>
#endif

namespace lum::rhi::gl {

	void GLDevice::Initialize( Window* window ) {

		GLFWwindow* w = static_cast<GLFWwindow*>(window->GetNativeWindow());

		glfwMakeContextCurrent(w);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			return;
		}

#		if LUM_ENABLE_IMGUI == 1
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGui_ImplGlfw_InitForOpenGL(w, true);
			ImGui_ImplOpenGL3_Init("#version 450");
#		endif
#		if LUM_ENABLE_DEBUG_RENDER == 1
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(rhi::detail::GLDebugCallback, nullptr);
#		endif

		mWindow = window;

	}

	void GLDevice::Draw( RVertexLayoutHandle layout, uint32 numVertices) {

		LUM_ASSERT(mLayouts.Contains(layout), "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[layout].mHandle);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);

		LUM_PROFILER_DRAW_CALL();

	}

	void GLDevice::DrawInstanced( RVertexLayoutHandle layout, uint32 numVertices, uint32 numInstances ) {

	}
	void GLDevice::DrawInstancedBase( RVertexLayoutHandle layout, uint32 numVertices, uint32 numInstances, uint32 baseInstance ) {

	}

	void GLDevice::DrawElements( RVertexLayoutHandle layout, uint32 numIndices ) {

		LUM_ASSERT(mLayouts.Contains(layout), "Cannot draw, invalid vertex layout");

		LUM_ASSERT(mBuffers.Contains(mLayouts[layout].mElementBuff), "Layout doesn't have attached any element buffers");

		glBindVertexArray(mLayouts[layout].mHandle);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(numIndices), GL_UNSIGNED_INT, nullptr);

		LUM_PROFILER_DRAW_CALL();

	}

	void GLDevice::DrawElementsInstanced( RVertexLayoutHandle layout, uint32 numIndices, uint32 numInstances ) {

	}
	void GLDevice::DrawElementsInstancedBase( RVertexLayoutHandle layout, uint32 numIndices, uint32 numInstances, uint32 baseInstance ) {

	}

	void GLDevice::NewFrame( ) {

#		if LUM_ENABLE_IMGUI == 1
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
#		endif

	}

	void GLDevice::SwapBuffers( ) {

#		if LUM_ENABLE_IMGUI == 1
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#		endif

		glfwSwapBuffers(static_cast<GLFWwindow*>(mWindow->GetNativeWindow()));
		
	}

}