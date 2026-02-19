//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL device state core management and rendering commands
//          Viewport, color/depth/stencil clearing, draw calls,
//          frame begin/end, and state queries
//
//=============================================================================//

#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/utils/asset_loader.hpp"

#include "rhi/rhi_pch.hpp"
#include "rhi/backend/opengl/gl_device.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_common.hpp"

#include "window_context/window.hpp"

#if LUM_ENABLE_IMGUI == 1
	#include <imgui.h>
	#include <imgui_impl_glfw.h>
	#include <imgui_impl_opengl3.h>
#endif

namespace lum::rhi::gl {

	void GLDevice::SetViewport ( int32 x, int32 y, int32 width, int32 height ) {

		if (mViewportState.x == x && mViewportState.y == y && mViewportState.mWidth == width && mViewportState.mHeight == height) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(x, y, width, height);

		mViewportState.x = x;
		mViewportState.y = y;
		mViewportState.mWidth = width;
		mViewportState.mHeight = height;

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
			mViewportState.mWidth,
			mViewportState.mHeight
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
			mViewportState.mWidth,
			mViewportState.mHeight
		);

		mViewportState.y = y;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetViewportWidth ( int32 width ) {

		if (mViewportState.mWidth == width) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(
			mViewportState.x,
			mViewportState.y,
			width,
			mViewportState.mHeight
		);

		mViewportState.mWidth = width;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetViewportHeight ( int32 height ) {

		if (mViewportState.mHeight == height) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(
			mViewportState.x,
			mViewportState.y,
			mViewportState.mWidth,
			height
		);
		
		mViewportState.mHeight = height;

		LUM_PROFILER_CACHE_MISS();

	}

	const RBlendState& GLDevice::GetBlendState ( ) const noexcept {
		return mBlendState;
	}
	const RCullState& GLDevice::GetCullState ( ) const noexcept {
		return mCullState;
	}
	const RScissorState& GLDevice::GetScissorState ( ) const noexcept {
		return mScissorState;
	}
	const RDepthStencilState& GLDevice::GetDepthStencilState ( ) const noexcept {
		return mDepthStencilState;
	}
	const RRasterizerState& GLDevice::GetRasterizerState ( ) const noexcept {
		return mRasterizerState;
	}
	const RViewportState& GLDevice::GetViewport ( ) const noexcept {
		return mViewportState;
	}

	bool GLDevice::IsValid ( RBufferHandle handle ) const {
		return mBuffers.exist(handle);
	}
	bool GLDevice::IsValid ( RTextureHandle handle ) const {
		return mTextures.exist(handle);
	}
	bool GLDevice::IsValid ( RShaderHandle handle ) const {
		return mShaders.exist(handle);
	}
	bool GLDevice::IsValid ( RFramebufferHandle handle ) const {
		return mFramebuffers.exist(handle);
	}
	bool GLDevice::IsValid ( RVertexLayoutHandle handle ) const {
		return mLayouts.exist(handle);
	}
	bool GLDevice::IsValid ( RPipelineHandle handle ) const {
		return mPipelines.exist(handle);
	}
	bool GLDevice::IsValid ( RSamplerHandle handle ) const {
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
	void GLDevice::SetColorMask ( RColorMask rgba ) {

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
	void GLDevice::Clear ( Flags<RClearFlag> flags ) {
		
		GLbitfield mask = 0;

		mask |= (flags.Has(RClearFlag::Color)) ? GL_COLOR_BUFFER_BIT : 0;
		mask |= (flags.Has(RClearFlag::Depth)) ? GL_DEPTH_BUFFER_BIT : 0;
		mask |= (flags.Has(RClearFlag::Stencil)) ? GL_STENCIL_BUFFER_BIT : 0;

		glClear(mask);

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::Draw(const RVertexLayoutHandle& vao, uint32 vertex_count) {

		LUM_HOTCHK_RETURN_VOID(mLayouts.exist(vao), LUM_SEV_WARN, "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].mHandle);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

		LUM_PROFILER_DRAW_CALL();

	}

	void GLDevice::DrawInstanced(const RVertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount) {

	}
	void GLDevice::DrawInstancedBase(const RVertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount, uint32 baseInstance) {

	}

	void GLDevice::DrawElements(const RVertexLayoutHandle& vao, uint32 indices_count) {
		LUM_HOTCHK_RETURN_VOID(mLayouts.exist(vao), LUM_SEV_WARN, "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].mHandle);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_count), GL_UNSIGNED_INT, nullptr);

		LUM_PROFILER_DRAW_CALL();

	}

	void GLDevice::DrawElementsInstanced ( const RVertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount) {

	}
	void GLDevice::DrawElementsInstancedBase ( const RVertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount, uint32 baseInstance) {

	}

	void GLDevice::BeginFrame() {

		LUM_PROFILER_BEGIN_FRAME();

		SetViewport(0, 0, window->GetWidth(), window->GetHeight());
		Clear(RClearFlag::Color | RClearFlag::Depth | RClearFlag::Stencil);

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

		LUM_PROFILER_END_FRAME();

	}

}