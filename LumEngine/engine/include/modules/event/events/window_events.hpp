#pragma once

#include "event/event_common.hpp"
#include "platform/input_common.hpp"

namespace lum {

	struct EWindowResized : public Event {

		uint32 mWidth;
		uint32 mHeight;

	};

	struct EWindowClosed	: public Event { };
	struct EKeyPressed		: public Event { input::Key mKey; };
	struct EKeyReleased		: public Event { input::Key mKey; };

} // namespace lum