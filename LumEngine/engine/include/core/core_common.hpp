#pragma once
#include "core/core_pch.hpp"
namespace lum {
	// GLOBAL ALIAS

	// Type for bits in lumengine
	using Bit = uint32_t;

	/// Generates ID for any type
	/// @tparam T object Type to generate an ID
	/// @tparam NULL_VAL Null value for ID's
	template<typename T, int NULL_VAL>
	struct GenerateID {
		inline static uint64_t Get() {
			return globalID++;
		}
	private:
		inline static uint64_t globalID = NULL_VAL + 1;
	};
	
	/// Converts any arithmetic type to float at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to float.
	template<typename T>
		requires std::is_arithmetic_v<T>
	constexpr float to_f(T value) { return static_cast<float>(value); }

	/// Converts any arithmetic type to unsigned int at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to unsigned int.
	template<typename T>
		requires std::is_arithmetic_v<T>
	constexpr float to_u(T value) { return static_cast<unsigned int>(value); }

	/// Converts any arithmetic type to uint32_t at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint32_t.
	template<typename T>
		requires std::is_arithmetic_v<T>
	constexpr float to_u32(T value) { return static_cast<uint32_t>(value); }

	/// Converts any arithmetic type to uint64_t at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint64_t.
	template<typename T>
		requires std::is_arithmetic_v<T>
	constexpr float to_u64(T value) { return static_cast<uint64_t>(value); }

	/// Converts any arithmetic type to double at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to double.
	template<typename T>
		requires std::is_arithmetic_v<T>
	constexpr float to_d(T value) { return static_cast<double>(value); }

	/// Converts any arithmetic type to int at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to int.
	template<typename T>
		requires std::is_arithmetic_v<T>
	constexpr float to_i(T value) { return static_cast<int>(value); }

	/// Calculates linear interpolation in time
	/// @tparam T Arithmetic type of calculations
	/// @param a First value
	/// @param b Second value
	/// @param t Time
	template<typename T>
		requires std::is_arithmetic_v<T>
	constexpr T lerp(T a, T b, float t) {
		return a + (b - a) * t;
	}

	/// Returns max value of arithmetic type.
	/// @tparam Arithmetic type.
	template<typename T>
	requires std::is_arithmetic_v<T>
	constexpr T max_val() {
		return std::numeric_limits<T>::max();
	}

	namespace settings {

		////////////////////////////////////////////////////////////////////////
		// ECS

		// Defines how much memory needs to be reserved for components
		inline constexpr size_t ECS_MAX_POOL_CAPACITY	= 1024;
		inline constexpr size_t ECS_MAX_ENTITY_COUNT	= 1024;

		// Defines how much containers needs to be reserved for components
		inline constexpr size_t ECS_MAX_COMPONENT_TYPES_COUNT = 32;

		////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////
		// Audio

		// Defines how much memory needs to be reserved for audio
		inline constexpr size_t AUDIO_MAX_SOUNDS_COUNT		= 256;
		inline constexpr size_t AUDIO_MAX_CHANNELS_COUNT	= 256;

		////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////
		// Events

		// Defines how much memory needs to be reserved for pools

		inline constexpr size_t EVENT_MAX_EVENT_TYPES = 32;

		// All settings under are for single event type

		inline constexpr size_t EVENT_MAX_CALLBACKS_PER_FRAME	= 8;
		inline constexpr size_t EVENT_MAX_PERMAMENT_CALLBACKS	= 8;
		inline constexpr size_t EVENT_MAX_EMITTS_PER_FRAME		= 64;

		////////////////////////////////////////////////////////////////////////


	}

	namespace cmdcolor {
		inline constexpr const char* reset		= "\033[0m";
		inline constexpr const char* black		= "\033[30m";
		inline constexpr const char* red		= "\033[31m";
		inline constexpr const char* green		= "\033[32m";
		inline constexpr const char* yellow		= "\033[33m";
		inline constexpr const char* blue		= "\033[34m";
		inline constexpr const char* magenta	= "\033[35m";
		inline constexpr const char* cyan		= "\033[36m";
		inline constexpr const char* white		= "\033[37m";

		inline constexpr const char* light_black	= "\033[90m";
		inline constexpr const char* light_red		= "\033[91m";
		inline constexpr const char* light_green	= "\033[92m";
		inline constexpr const char* light_yellow	= "\033[93m";
		inline constexpr const char* light_blue		= "\033[94m";
		inline constexpr const char* light_magenta	= "\033[95m";
		inline constexpr const char* light_cyan		= "\033[96m";
		inline constexpr const char* light_white	= "\033[97m";

		inline constexpr const char* bg_black		= "\033[40m";
		inline constexpr const char* bg_red			= "\033[41m";
		inline constexpr const char* bg_green		= "\033[42m";
		inline constexpr const char* bg_yellow		= "\033[43m";
		inline constexpr const char* bg_blue		= "\033[44m";
		inline constexpr const char* bg_magenta		= "\033[45m";
		inline constexpr const char* bg_cyan		= "\033[46m";
		inline constexpr const char* bg_white		= "\033[47m";

		inline constexpr const char* bg_light_black		= "\033[100m";
		inline constexpr const char* bg_light_red		= "\033[101m";
		inline constexpr const char* bg_light_green		= "\033[102m";
		inline constexpr const char* bg_light_yellow	= "\033[103m";
		inline constexpr const char* bg_light_blue		= "\033[104m";
		inline constexpr const char* bg_light_magenta	= "\033[105m";
		inline constexpr const char* bg_light_cyan		= "\033[106m";
		inline constexpr const char* bg_light_white		= "\033[107m";

		inline constexpr const char* bold			= "\033[1m";
		inline constexpr const char* italic			= "\033[3m";
		inline constexpr const char* underline		= "\033[4m";
		inline constexpr const char* strikethrough	= "\033[9m";
		inline constexpr const char* reverse		= "\033[7m";
	}
}