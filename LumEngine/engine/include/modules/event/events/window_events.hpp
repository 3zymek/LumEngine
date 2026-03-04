#pragma once

#include "event/event_common.hpp"

namespace lum {

	struct EWindowResized : public Event {
		
		uint32 mWidth;
		uint32 mHeight;

	};

	struct EWindowClosed : public Event {};

} // namespace lum