#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
namespace lum {

	// GLOBAL ALIAS

	using LUMdouble = double;
	using LUMfloat	= float;
	using LUMint	= int;
	using LUMuint	= uint32_t;
	using LUMbyte	= uint8_t;
	using LUMbool	= bool;
	using LUMushort = uint16_t;
	using LUMsize	= size_t;
	using LUMstring = std::string;
	using LUMstrview = std::string_view;


	// Alias for void*
	using LUMvptr = void*;
	// Alias for const void*
	using LUMcvptr = const void*;

	// Alias for const char*
	using LUMcharptr = const char*;
	using LUMstr = char*;

	// Type for bits in lumengine
	using LUMbitfield = uint32_t;

	/// Generates ID for any type
	/// @tparam T object Type to generate an ID
	/// @tparam NULL_VAL Null value for ID's
	template<typename T, int NULL_VAL>
	struct GenerateID {
		static LUMuint Get() {
			return globalID++;
		}
	private:
		inline static LUMuint globalID = NULL_VAL + 1;
	};
	
	/// Converts any arithmetic type to float at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to float.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr LUMfloat to_f(T value) { return static_cast<LUMfloat>(value); }

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
	inline constexpr LUMuint to_u32(T value) { return static_cast<LUMuint>(value); }

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
	inline constexpr LUMdouble to_d(T value) { return static_cast<LUMdouble>(value); }

	/// Converts any arithmetic type to int at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to int.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr LUMint to_i(T value) { return static_cast<LUMint>(value); }

	/// Calculates linear interpolation in time
	/// @tparam T Arithmetic type of calculations
	/// @param a First value
	/// @param b Second value
	/// @param t Time
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr T lerp(T a, T b, LUMfloat t) {
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
		LUM_CONST_VAR_QUALIFIER LUMcharptr reset		= "\033[0m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr black		= "\033[30m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr red		= "\033[31m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr green		= "\033[32m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr yellow		= "\033[33m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr blue		= "\033[34m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr magenta	= "\033[35m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr cyan		= "\033[36m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr white		= "\033[37m";

		LUM_CONST_VAR_QUALIFIER LUMcharptr light_black	= "\033[90m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr light_red		= "\033[91m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr light_green	= "\033[92m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr light_yellow	= "\033[93m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr light_blue		= "\033[94m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr light_magenta	= "\033[95m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr light_cyan		= "\033[96m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr light_white	= "\033[97m";

		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_black		= "\033[40m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_red			= "\033[41m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_green		= "\033[42m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_yellow		= "\033[43m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_blue		= "\033[44m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_magenta		= "\033[45m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_cyan		= "\033[46m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_white		= "\033[47m";

		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_light_black		= "\033[100m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_light_red		= "\033[101m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_light_green		= "\033[102m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_light_yellow	= "\033[103m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_light_blue		= "\033[104m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_light_magenta	= "\033[105m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_light_cyan		= "\033[106m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr bg_light_white		= "\033[107m";

		LUM_CONST_VAR_QUALIFIER LUMcharptr bold			= "\033[1m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr italic			= "\033[3m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr underline		= "\033[4m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr strikethrough	= "\033[9m";
		LUM_CONST_VAR_QUALIFIER LUMcharptr reverse		= "\033[7m";
	}
}