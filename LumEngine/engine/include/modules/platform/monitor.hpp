#pragma once
#include "core/core_common.hpp"
#include "rhi/rhi_pch.hpp"

namespace lum {

	class Monitor {
	public:

		void Initialize( ) {
			mMonitor = glfwGetPrimaryMonitor( );
			mVideoMode = glfwGetVideoMode( mMonitor );
		}

		int32 GetWidth( )       const noexcept { return mVideoMode->width; }
		int32 GetHeight( )      const noexcept { return mVideoMode->height; }
		int32 GetRefreshRate( ) const noexcept { return mVideoMode->refreshRate; }

		void GetWorkArea( int32& x, int32& y, int32& w, int32& h ) const noexcept {
			glfwGetMonitorWorkarea( mMonitor, &x, &y, &w, &h );
		}

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

		GLFWmonitor* mMonitor = nullptr;
		const GLFWvidmode* mVideoMode = nullptr;

	};
}