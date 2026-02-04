#include "rhi/backend/opengl/gl_device.hpp"
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "core/asset_service.hpp"
#include "window_context/window.hpp"
#include "rhi/rhi_common.hpp"
#include "core/core_pch.hpp"
#if LUM_ENABLE_IMGUI == 1
	#include "imgui.h"
	#include "imgui_impl_glfw.h"
	#include "imgui_impl_opengl3.h"
#endif
namespace lum::rhi::gl {

	///////////////////////////////////////////////////
	/// Other
	///////////////////////////////////////////////////

	void GLDevice::Draw(const VertexLayoutHandle& vao, uint32 vertex_count) {

		LUM_HOTCHK_RETURN_VOID(mLayouts.exist(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

	}
	void GLDevice::DrawElements(const VertexLayoutHandle& vao, uint32 indices_count) {
		LUM_HOTCHK_RETURN_VOID(mLayouts.exist(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].vao);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_count), GL_UNSIGNED_INT, nullptr);

	}
	void GLDevice::BeginFrame() {

		glViewport(0, 0, window->GetWidth(), window->GetHeight());
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

#		if LUM_ENABLE_IMGUI == 1
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#		endif

		glfwPollEvents();

	}
	void GLDevice::EndFrame() {

#		if LUM_ENABLE_IMGUI == 1
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


#		endif

		glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetNativeWindow()));

	}

}