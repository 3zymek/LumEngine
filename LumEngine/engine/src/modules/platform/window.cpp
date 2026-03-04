//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Platform window abstraction for the Lum engine.
//
//=============================================================================//

#include "platform/window.hpp"
#include "event/event_bus.hpp"
#include "core/utils/lum_assert.hpp"
#include "event/events/window_events.hpp"

namespace lum {

	void Window::Initialize( const WindowDescriptor& desc ) {
		
		mEventBus = desc.mEventBus;

		init(desc);

	}

	void Window::SetWidth( uint32 width ) {

		mWidth = width;
		glfwSetWindowSize(mWindow, mWidth, mHeight);

	}
	void Window::SetHeight( uint32 height ) {

		mHeight = height;
		glfwSetWindowSize(mWindow, mWidth, mHeight);

	}
	uint32 Window::GetWidth( ) const noexcept {

		if (!mWindow) return 0;
		int32 w, h;
		glfwGetWindowSize(mWindow, &w, &h);
		return w;

	}

	uint32 Window::GetHeight( ) const noexcept {

		if (!mWindow) return 0;
		int32 w, h;
		glfwGetWindowSize(mWindow, &w, &h);
		return h;

	}
	vptr Window::GetNativeWindow( ) const noexcept {

		return mWindow;

	}
	void Window::Update( ) noexcept {

		int32 width, height;
		glfwGetWindowSize(mWindow, &width, &height);
		if (width != mWidth || height != mHeight) {
			EWindowResized ev;

			mWidth = width;
			mHeight = height;

			ev.mWidth = width;
			ev.mHeight = height;

			mEventBus->Emit(ev);
		}

		glfwPollEvents();

	}
	bool Window::IsOpen( ) const noexcept {

		return !glfwWindowShouldClose(mWindow);

	}

	void Window::init( const WindowDescriptor& desc ) {

		if (!glfwInit()) {
			LUM_LOG_FATAL("Failed to initialize GLFW");
			return;
		}

		mWindow = glfwCreateWindow(desc.mWidth, desc.mHeight, desc.mTitle.c_str(), nullptr, nullptr);

		mWidth = desc.mWidth;
		mHeight = desc.mHeight;

		if (!mWindow) {
			glfwTerminate();
			return;
		}

	}

}