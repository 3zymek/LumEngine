#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/types.hpp"
namespace lum {

	/// Generates ID for any type
	/// @tparam T object Type to generate an ID
	/// @tparam NULL_VAL Null value for ID's
	template<typename T, int NULL_VAL>
	struct GenerateID {
		static uint32 get() {
			return globalID++;
		}
	private:
		inline static uint32 globalID = NULL_VAL + 1;
	};
	
	/// Converts any arithmetic type to float at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to float.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr float32 to_f(T value) { return static_cast<float32>(value); }

	/// Converts any arithmetic type to uint8 at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint8.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint8 to_u8(T value) { return static_cast<uint8>(value); }

	/// Converts any arithmetic type to uint16 at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint16.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint16 to_u16(T value) { return static_cast<uint16>(value); }

	/// Converts any arithmetic type to uint32_t at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint32_t.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint32 to_u32(T value) { return static_cast<uint32>(value); }

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
	inline constexpr float64 to_d(T value) { return static_cast<float64>(value); }

	/// Converts any arithmetic type to int32 at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to int32.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr int32 to_i(T value) { return static_cast<int32>(value); }

	/// Calculates linear interpolation in time
	/// @tparam T Arithmetic type of calculations
	/// @param a First value
	/// @param b Second value
	/// @param t Time
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr T lerp(T a, T b, float32 t) {
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
	inline constexpr usize bytesize(const std::vector<T>& vector) {
		return sizeof(T) * vector.size();
	}

	/// @return Returns definitive size of type in bytes multiplied by size.
	/// @param arr Pointer to the data.
	/// @param size How much elements.
	template<typename T, usize L>
	inline constexpr usize bytesize(const T (&arr)[L]) {
		return sizeof(T) * L;
	}

	/// @return Returns definitive size of type in bytes multiplied by size.
	/// @param arr Pointer to the data.
	/// @param size How much elements.
	template<typename T>
	inline constexpr usize bytesize(const T* arr, usize size) {
		return sizeof(T) * size;
	}

	/// @return Underlying type of enum.
	/// @param e Enum.
	template<typename T>
		requires std::is_enum_v<T>
	inline constexpr std::underlying_type_t<T> to_underlying(T e){
		return static_cast<std::underlying_type_t<T>>(e);
	}


	namespace settings {

		////////////////////////////////////////////////////////////////////////
		// ECS

		// Defines how much memory needs to be reserved for components
		LUM_CONST_VAR_QUALIFIER usize ECS_MAX_POOL_CAPACITY	= 1024;
		LUM_CONST_VAR_QUALIFIER usize ECS_MAX_ENTITY_COUNT	= 1024;

		// Defines how much containers needs to be reserved for components
		LUM_CONST_VAR_QUALIFIER usize ECS_MAX_COMPONENT_TYPES_COUNT = 32;

		////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////
		// Audio

		// Defines how much memory needs to be reserved for audio
		LUM_CONST_VAR_QUALIFIER usize AUDIO_MAX_SOUNDS_COUNT		= 256;
		LUM_CONST_VAR_QUALIFIER usize AUDIO_MAX_CHANNELS_COUNT	= 256;

		////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////
		// Events

		// Defines how much memory needs to be reserved for pools

		LUM_CONST_VAR_QUALIFIER usize EVENT_MAX_EVENT_TYPES = 32;

		// All settings under are for single event type

		LUM_CONST_VAR_QUALIFIER usize EVENT_MAX_CALLBACKS_PER_FRAME	= 8;
		LUM_CONST_VAR_QUALIFIER usize EVENT_MAX_PERMAMENT_CALLBACKS	= 8;
		LUM_CONST_VAR_QUALIFIER usize EVENT_MAX_EMITTS_PER_FRAME		= 64;

		////////////////////////////////////////////////////////////////////////


	}

	namespace cmdcolor {
		LUM_CONST_VAR_QUALIFIER ccharptr reset		= "\033[0m";
		LUM_CONST_VAR_QUALIFIER ccharptr black		= "\033[30m";
		LUM_CONST_VAR_QUALIFIER ccharptr red		= "\033[31m";
		LUM_CONST_VAR_QUALIFIER ccharptr green		= "\033[32m";
		LUM_CONST_VAR_QUALIFIER ccharptr yellow		= "\033[33m";
		LUM_CONST_VAR_QUALIFIER ccharptr blue		= "\033[34m";
		LUM_CONST_VAR_QUALIFIER ccharptr magenta	= "\033[35m";
		LUM_CONST_VAR_QUALIFIER ccharptr cyan		= "\033[36m";
		LUM_CONST_VAR_QUALIFIER ccharptr white		= "\033[37m";

		LUM_CONST_VAR_QUALIFIER ccharptr light_black	= "\033[90m";
		LUM_CONST_VAR_QUALIFIER ccharptr light_red		= "\033[91m";
		LUM_CONST_VAR_QUALIFIER ccharptr light_green	= "\033[92m";
		LUM_CONST_VAR_QUALIFIER ccharptr light_yellow	= "\033[93m";
		LUM_CONST_VAR_QUALIFIER ccharptr light_blue		= "\033[94m";
		LUM_CONST_VAR_QUALIFIER ccharptr light_magenta	= "\033[95m";
		LUM_CONST_VAR_QUALIFIER ccharptr light_cyan		= "\033[96m";
		LUM_CONST_VAR_QUALIFIER ccharptr light_white	= "\033[97m";

		LUM_CONST_VAR_QUALIFIER ccharptr bg_black		= "\033[40m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_red			= "\033[41m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_green		= "\033[42m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_yellow		= "\033[43m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_blue		= "\033[44m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_magenta		= "\033[45m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_cyan		= "\033[46m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_white		= "\033[47m";

		LUM_CONST_VAR_QUALIFIER ccharptr bg_light_black		= "\033[100m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_light_red		= "\033[101m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_light_green		= "\033[102m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_light_yellow	= "\033[103m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_light_blue		= "\033[104m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_light_magenta	= "\033[105m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_light_cyan		= "\033[106m";
		LUM_CONST_VAR_QUALIFIER ccharptr bg_light_white		= "\033[107m";

		LUM_CONST_VAR_QUALIFIER ccharptr bold			= "\033[1m";
		LUM_CONST_VAR_QUALIFIER ccharptr italic			= "\033[3m";
		LUM_CONST_VAR_QUALIFIER ccharptr underline		= "\033[4m";
		LUM_CONST_VAR_QUALIFIER ccharptr strikethrough	= "\033[9m";
		LUM_CONST_VAR_QUALIFIER ccharptr reverse		= "\033[7m";
	}
}