#pragma once

#include "platform/window.hpp"

namespace lum::editor {

	class WindowTitlebar {
	public:

		void Draw( Window* window );

	private:

		bool bMaximized = false;
		int32 mLastX = 0;
		int32 mLastY = 0;
		int32 mLastWidth = 0;
		int32 mLastHeight = 0;

	};

}