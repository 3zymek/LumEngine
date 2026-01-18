#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
namespace lum {

	// GLOBAL ALIAS

	// Alias for void*
	using vptr_t = void*;
	// Alias for const void*
	using cvptr_t = const void*;

	// Alias for const char*
	using cstr_t = const char*;

	// Type for bits in lumengine
	using bit_t = uint32_t;

	/// Generates ID for any type
	/// @tparam T object Type to generate an ID
	/// @tparam NULL_VAL Null value for ID's
	template<typename T, int NULL_VAL>
	struct GenerateID {
		static uint64_t Get() {
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
	inline constexpr float to_f(T value) { return static_cast<float>(value); }

	/// Converts any arithmetic type to unsigned int at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to unsigned int.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr unsigned int to_u(T value) { return static_cast<unsigned int>(value); }

	/// Converts any arithmetic type to uint32_t at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint32_t.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint32_t to_u32(T value) { return static_cast<uint32_t>(value); }

	/// Converts any arithmetic type to uint64_t at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint64_t.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint64_t to_u64(T value) { return static_cast<uint64_t>(value); }

	/// Converts any arithmetic type to double at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to double.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr float to_d(T value) { return static_cast<double>(value); }

	/// Converts any arithmetic type to int at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to int.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr int to_i(T value) { return static_cast<int>(value); }

	/// Calculates linear interpolation in time
	/// @tparam T Arithmetic type of calculations
	/// @param a First value
	/// @param b Second value
	/// @param t Time
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr T lerp(T a, T b, float t) {
		return a + (b - a) * t;
	}

	/// Returns max value of arithmetic type.
	/// @tparam T arithmetic type.
	template<typename T>
	requires std::is_arithmetic_v<T>
	inline constexpr T max_val() {
		return std::numeric_limits<T>::max();
	}

	/// @return Returns definitive size of vector in bytes.
	/// @param vector Vector to calculate size.
	template<typename T>
	inline constexpr size_t bytesize(const std::vector<T>& vector) {
		return sizeof(T) * vector.size();
	}

	/// @return Returns definitive size of type in bytes multiplied by size.
	/// @param arr Pointer to the data.
	/// @param size How much elements.
	template<typename T>
	inline constexpr size_t bytesize(const T* arr, size_t size) {
		return sizeof(T) * size;
	}

	namespace settings {

		////////////////////////////////////////////////////////////////////////
		// ECS

		// Defines how much memory needs to be reserved for components
		LUM_CONST_VAR_QUALIFIER size_t ECS_MAX_POOL_CAPACITY	= 1024;
		LUM_CONST_VAR_QUALIFIER size_t ECS_MAX_ENTITY_COUNT	= 1024;

		// Defines how much containers needs to be reserved for components
		LUM_CONST_VAR_QUALIFIER size_t ECS_MAX_COMPONENT_TYPES_COUNT = 32;

		////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////
		// Audio

		// Defines how much memory needs to be reserved for audio
		LUM_CONST_VAR_QUALIFIER size_t AUDIO_MAX_SOUNDS_COUNT		= 256;
		LUM_CONST_VAR_QUALIFIER size_t AUDIO_MAX_CHANNELS_COUNT	= 256;

		////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////
		// Events

		// Defines how much memory needs to be reserved for pools

		LUM_CONST_VAR_QUALIFIER size_t EVENT_MAX_EVENT_TYPES = 32;

		// All settings under are for single event type

		LUM_CONST_VAR_QUALIFIER size_t EVENT_MAX_CALLBACKS_PER_FRAME	= 8;
		LUM_CONST_VAR_QUALIFIER size_t EVENT_MAX_PERMAMENT_CALLBACKS	= 8;
		LUM_CONST_VAR_QUALIFIER size_t EVENT_MAX_EMITTS_PER_FRAME		= 64;

		////////////////////////////////////////////////////////////////////////


	}

	namespace cmdcolor {
		LUM_CONST_VAR_QUALIFIER cstr_t reset		= "\033[0m";
		LUM_CONST_VAR_QUALIFIER cstr_t black		= "\033[30m";
		LUM_CONST_VAR_QUALIFIER cstr_t red		= "\033[31m";
		LUM_CONST_VAR_QUALIFIER cstr_t green		= "\033[32m";
		LUM_CONST_VAR_QUALIFIER cstr_t yellow		= "\033[33m";
		LUM_CONST_VAR_QUALIFIER cstr_t blue		= "\033[34m";
		LUM_CONST_VAR_QUALIFIER cstr_t magenta	= "\033[35m";
		LUM_CONST_VAR_QUALIFIER cstr_t cyan		= "\033[36m";
		LUM_CONST_VAR_QUALIFIER cstr_t white		= "\033[37m";

		LUM_CONST_VAR_QUALIFIER cstr_t light_black	= "\033[90m";
		LUM_CONST_VAR_QUALIFIER cstr_t light_red		= "\033[91m";
		LUM_CONST_VAR_QUALIFIER cstr_t light_green	= "\033[92m";
		LUM_CONST_VAR_QUALIFIER cstr_t light_yellow	= "\033[93m";
		LUM_CONST_VAR_QUALIFIER cstr_t light_blue		= "\033[94m";
		LUM_CONST_VAR_QUALIFIER cstr_t light_magenta	= "\033[95m";
		LUM_CONST_VAR_QUALIFIER cstr_t light_cyan		= "\033[96m";
		LUM_CONST_VAR_QUALIFIER cstr_t light_white	= "\033[97m";

		LUM_CONST_VAR_QUALIFIER cstr_t bg_black		= "\033[40m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_red			= "\033[41m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_green		= "\033[42m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_yellow		= "\033[43m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_blue		= "\033[44m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_magenta		= "\033[45m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_cyan		= "\033[46m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_white		= "\033[47m";

		LUM_CONST_VAR_QUALIFIER cstr_t bg_light_black		= "\033[100m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_light_red		= "\033[101m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_light_green		= "\033[102m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_light_yellow	= "\033[103m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_light_blue		= "\033[104m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_light_magenta	= "\033[105m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_light_cyan		= "\033[106m";
		LUM_CONST_VAR_QUALIFIER cstr_t bg_light_white		= "\033[107m";

		LUM_CONST_VAR_QUALIFIER cstr_t bold			= "\033[1m";
		LUM_CONST_VAR_QUALIFIER cstr_t italic			= "\033[3m";
		LUM_CONST_VAR_QUALIFIER cstr_t underline		= "\033[4m";
		LUM_CONST_VAR_QUALIFIER cstr_t strikethrough	= "\033[9m";
		LUM_CONST_VAR_QUALIFIER cstr_t reverse		= "\033[7m";
	}
}