#pragma once

#include "event/event_common.hpp"

namespace lum {

	struct EWindowResized : public Event {
		
		uint32 mWidth = 0;
		uint32 mHeight = 0;

	};

	struct EWindowClosed : public Event {};

} // namespace lum