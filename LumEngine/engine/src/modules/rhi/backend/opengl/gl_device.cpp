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

	void GLDevice::SetViewport ( int32 x, int32 y, int32 width, int32 height ) {

		if (mViewportState.x == x && mViewportState.y == y && mViewportState.width == width && mViewportState.height == height) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(x, y, width, height);

		mViewportState.x = x;
		mViewportState.y = y;
		mViewportState.width = width;
		mViewportState.height = height;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetViewportX ( int32 x ) {
		
		if (mViewportState.x == x) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(
			x,
			mViewportState.y,
			mViewportState.width,
			mViewportState.height
		);

		mViewportState.x = x;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetViewportY ( int32 y ) {

		if (mViewportState.y == y) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(
			mViewportState.x,
			y,
			mViewportState.width,
			mViewportState.height
		);

		mViewportState.y = y;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetViewportWidth ( int32 width ) {

		if (mViewportState.width == width) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(
			mViewportState.x,
			mViewportState.y,
			width,
			mViewportState.height
		);

		mViewportState.width = width;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetViewportHeight ( int32 height ) {

		if (mViewportState.height == height) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(
			mViewportState.x,
			mViewportState.y,
			mViewportState.width,
			height
		);
		
		mViewportState.height = height;

		LUM_PROFILER_CACHE_MISS();

	}

	const BlendState& GLDevice::GetBlendState ( ) const noexcept {
		return mBlendState;
	}
	const CullState& GLDevice::GetCullState ( ) const noexcept {
		return mCullState;
	}
	const ScissorState& GLDevice::GetScissorState ( ) const noexcept {
		return mScissorState;
	}
	const DepthStencilState& GLDevice::GetDepthStencilState ( ) const noexcept {
		return mDepthStencilState;
	}
	const RasterizerState& GLDevice::GetRasterizerState ( ) const noexcept {
		return mRasterizerState;
	}
	const ViewportState& GLDevice::GetViewport ( ) const noexcept {
		return mViewportState;
	}

	bool GLDevice::IsValid ( BufferHandle handle ) const {
		return mBuffers.exist(handle);
	}
	bool GLDevice::IsValid ( TextureHandle handle ) const {
		return mTextures.exist(handle);
	}
	bool GLDevice::IsValid ( ShaderHandle handle ) const {
		return mShaders.exist(handle);
	}
	bool GLDevice::IsValid ( FramebufferHandle handle ) const {
		return mFramebuffers.exist(handle);
	}
	bool GLDevice::IsValid ( VertexLayoutHandle handle ) const {
		return mLayouts.exist(handle);
	}
	bool GLDevice::IsValid ( PipelineHandle handle ) const {
		return mPipelines.exist(handle);
	}
	bool GLDevice::IsValid ( SamplerHandle handle ) const {
		return mSamplers.exist(handle);
	}

	void GLDevice::SetColorMask ( bool r, bool g, bool b, bool a ) {

		if (r == mColorMask.r &&
			g == mColorMask.g &&
			b == mColorMask.b &&
			a == mColorMask.a)
		{
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glColorMask(
			r ? GL_TRUE : GL_FALSE,
			g ? GL_TRUE : GL_FALSE,
			b ? GL_TRUE : GL_FALSE,
			a ? GL_TRUE : GL_FALSE
		);

		mColorMask.r = r;
		mColorMask.g = g;
		mColorMask.b = b;
		mColorMask.a = a;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetColorMask ( ColorMask rgba ) {

		if (rgba.r == mColorMask.r &&
			rgba.g == mColorMask.g &&
			rgba.b == mColorMask.b &&
			rgba.a == mColorMask.a)
		{
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glColorMask(
			rgba.r ? GL_TRUE : GL_FALSE,
			rgba.g ? GL_TRUE : GL_FALSE,
			rgba.b ? GL_TRUE : GL_FALSE,
			rgba.a ? GL_TRUE : GL_FALSE
		);

		mColorMask.r = rgba.r;
		mColorMask.g = rgba.g;
		mColorMask.b = rgba.b;
		mColorMask.a = rgba.a;

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::SetClearColor ( ChannelRGBA color ) {

		if (mClearColor == color) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glClearColor(color.r, color.g, color.b, color.a);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::ClearColor ( ) {

		glClear(GL_COLOR_BUFFER_BIT);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::ClearColor ( ChannelRGBA color ) {

		SetClearColor(color);
		glClear(GL_COLOR_BUFFER_BIT);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::ClearDepth ( ) {

		glClear(GL_DEPTH_BUFFER_BIT);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::ClearStencil ( ) {

		glClear(GL_STENCIL_BUFFER_BIT);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::Clear ( Flags<ClearFlag> flags ) {
		
		GLbitfield mask = 0;

		mask |= (flags.has(ClearFlag::Color)) ? GL_COLOR_BUFFER_BIT : 0;
		mask |= (flags.has(ClearFlag::Depth)) ? GL_DEPTH_BUFFER_BIT : 0;
		mask |= (flags.has(ClearFlag::Stencil)) ? GL_STENCIL_BUFFER_BIT : 0;

		glClear(mask);

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::Draw(const VertexLayoutHandle& vao, uint32 vertex_count) {

		LUM_HOTCHK_RETURN_VOID(mLayouts.exist(vao), LUM_SEV_WARN, "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

		LUM_PROFILER_DRAW_CALL();

	}

	void GLDevice::DrawInstanced(const VertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount) {

	}
	void GLDevice::DrawInstancedBase(const VertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount, uint32 baseInstance) {

	}

	void GLDevice::DrawElements(const VertexLayoutHandle& vao, uint32 indices_count) {
		LUM_HOTCHK_RETURN_VOID(mLayouts.exist(vao), LUM_SEV_WARN, "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].vao);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_count), GL_UNSIGNED_INT, nullptr);

		LUM_PROFILER_DRAW_CALL();

	}

	void GLDevice::DrawElementsInstanced(const VertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount) {

	}
	void GLDevice::DrawElementsInstancedBase(const VertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount, uint32 baseInstance) {

	}

	void GLDevice::BeginFrame() {

		LUM_PROFILER_BEGIN_FRAME();

		SetViewport(0, 0, window->get_width(), window->get_height());
		Clear(ClearFlag::Color | ClearFlag::Depth | ClearFlag::Stencil);

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

		glfwSwapBuffers(static_cast<GLFWwindow*>(window->get_native_window()));

		LUM_PROFILER_END_FRAME();

	}

}