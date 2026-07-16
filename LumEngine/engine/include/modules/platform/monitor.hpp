//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Wraps GLFW monitor queries for resolution, refresh rate and work area.
//
//=============================================================================//
#pragma once
#include "Core/CoreCommon.hpp"
#include "Rhi/RhiPch.hpp"

namespace lum {

	/* @brief Wraps GLFW monitor queries for the primary monitor.
	*
	* Provides resolution, refresh rate and work area information.
	* Must be initialized after GLFW is initialized.
	*/
	class Monitor {
	public:

		/* @brief Queries the primary monitor and caches its video mode.
		*  Must be called after glfwInit().
		*/
		void Initialize( ) {
			mMonitor = glfwGetPrimaryMonitor( );
			mVideoMode = glfwGetVideoMode( mMonitor );
		}

		/* @brief Returns the width of the primary monitor in screen coordinates. */
		int32 GetWidth( ) const noexcept { return mVideoMode->width; }

		/* @brief Returns the height of the primary monitor in screen coordinates. */
		int32 GetHeight( ) const noexcept { return mVideoMode->height; }

		/* @brief Returns the refresh rate of the primary monitor in Hz. */
		int32 GetRefreshRate( ) const noexcept { return mVideoMode->refreshRate; }

		/* @brief Returns the usable work area of the primary monitor, excluding taskbars.
		*  @param x Left edge of the work area in screen coordinates.
		*  @param y Top edge of the work area in screen coordinates.
		*  @param w Width of the work area in screen coordinates.
		*  @param h Height of the work area in screen coordinates.
		*/
		void GetWorkArea( int32& x, int32& y, int32& w, int32& h ) const noexcept {
			glfwGetMonitorWorkarea( mMonitor, &x, &y, &w, &h );
		}

		/* @brief Returns the monitor that contains the top-left corner of the given window.
		*  Falls back to the primary monitor if no containing monitor is found.
		*  @param window GLFW window to query.
		*  @return Pointer to the GLFWmonitor containing the window.
		*/
		static GLFWmonitor* GetWindowMonitor( GLFWwindow* window ) {
			int32 wx, wy, ww, wh;
			glfwGetWindowPos( window, &wx, &wy );
			glfwGetWindowSize( window, &ww, &wh );

			int32 count;
			GLFWmonitor** monitors = glfwGetMonitors( &count );

			for (int32 i = 0; i < count; i++) {
				int32 mx, my, mw, mh;
				glfwGetMonitorWorkarea( monitors[ i ], &mx, &my, &mw, &mh );

				if (wx >= mx && wx < mx + mw && wy >= my && wy < my + mh)
					return monitors[ i ];
			}

			return glfwGetPrimaryMonitor( );
		}

	private:

		/* @brief Handle to the primary GLFW monitor. */
		GLFWmonitor* mMonitor = nullptr;

		/* @brief Cached video mode for the primary monitor. */
		const GLFWvidmode* mVideoMode = nullptr;

	};

} // namespace lum