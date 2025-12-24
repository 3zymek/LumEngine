#pragma once
#include "core/core_pch.hpp"

// GLOBAL ALIAS

using Bit = uint64_t;

template<typename T, int NULL_VAL>
struct GenerateID {
	inline static T Get() {
		return globalID++;
	}
private:
	inline static T globalID = NULL_VAL + 1;
};

namespace settings {

	////////////////////////////////////////////////////////////////////////
	// ECS

	// Defines how much memory needs to be reserved for components
	inline constexpr unsigned int MAX_POOL_CAPACITY = 1000000;
	inline constexpr unsigned int MAX_ENTITY_COUNT	= 1000000;

	// Defines how much containers needs to be reserved for components
	inline constexpr unsigned int MAX_COMPONENT_TYPES_COUNT = 10;

	////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////
	// Audio

	// Defines how much memory needs to be reserved for audio
	inline constexpr unsigned int MAX_SOUNDS_COUNT = 1000;
	inline constexpr unsigned int MAX_CHANNELS_COUNT = 700;

	////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////
	// Events

	// Defines how much containers needs to be reserved for events
	inline constexpr unsigned int MAX_EVENT_TYPES = 20;

	////////////////////////////////////////////////////////////////////////


}

namespace cmdcolor {
	inline constexpr const char* reset = "\033[0m";
	inline constexpr const char* black = "\033[30m";
	inline constexpr const char* red = "\033[31m";
	inline constexpr const char* green = "\033[32m";
	inline constexpr const char* yellow = "\033[33m";
	inline constexpr const char* blue = "\033[34m";
	inline constexpr const char* magenta = "\033[35m";
	inline constexpr const char* cyan = "\033[36m";  // poprawione
	inline constexpr const char* white = "\033[37m";

	inline constexpr const char* light_black = "\033[90m";
	inline constexpr const char* light_red = "\033[91m";
	inline constexpr const char* light_green = "\033[92m";
	inline constexpr const char* light_yellow = "\033[93m";
	inline constexpr const char* light_blue = "\033[94m";
	inline constexpr const char* light_magenta = "\033[95m";
	inline constexpr const char* light_cyan = "\033[96m";
	inline constexpr const char* light_white = "\033[97m";

	inline constexpr const char* bg_black = "\033[40m";
	inline constexpr const char* bg_red = "\033[41m";
	inline constexpr const char* bg_green = "\033[42m";
	inline constexpr const char* bg_yellow = "\033[43m";
	inline constexpr const char* bg_blue = "\033[44m";
	inline constexpr const char* bg_magenta = "\033[45m";
	inline constexpr const char* bg_cyan = "\033[46m";
	inline constexpr const char* bg_white = "\033[47m";

	inline constexpr const char* bg_light_black = "\033[100m";
	inline constexpr const char* bg_light_red = "\033[101m";
	inline constexpr const char* bg_light_green = "\033[102m";
	inline constexpr const char* bg_light_yellow = "\033[103m";
	inline constexpr const char* bg_light_blue = "\033[104m";
	inline constexpr const char* bg_light_magenta = "\033[105m";
	inline constexpr const char* bg_light_cyan = "\033[106m";
	inline constexpr const char* bg_light_white = "\033[107m";

	inline constexpr const char* bold = "\033[1m";
	inline constexpr const char* italic = "\033[3m";
	inline constexpr const char* underline = "\033[4m";
	inline constexpr const char* strikethrough = "\033[9m";
	inline constexpr const char* reverse = "\033[7m";
}