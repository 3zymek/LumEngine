#pragma once
#include "core/core_pch.hpp"
#include "core/types.hpp"
#include "core/utils/string_hasher.hpp"
#include "core/math/types.hpp"
namespace lum {

	/// Generates ID for any type
	/// @tparam T object Type to generate an ID
	/// @tparam NULL_VAL Null value for ID's
	template<typename T, int NULL_VAL>
	struct GenerateID {
		static uint32 Get() {
			return gID++;
		}
	private:
		inline static uint32 gID = NULL_VAL + 1;
	};
	
	/// Converts any arithmetic type to float at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to float.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr float32 ToF(T value) { return static_cast<float32>(value); }

	/// Converts any arithmetic type to uint8 at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint8.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint8 ToU8(T value) { return static_cast<uint8>(value); }

	/// Converts any arithmetic type to uint16 at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint16.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint16 ToU16(T value) { return static_cast<uint16>(value); }

	/// Converts any arithmetic type to uint32_t at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint32_t.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint32 ToU32(T value) { return static_cast<uint32>(value); }

	/// Converts any arithmetic type to uint64_t at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint64_t.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint64_t ToU64(T value) { return static_cast<uint64_t>(value); }

	/// Converts any arithmetic type to double at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to double.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr float64 ToD(T value) { return static_cast<float64>(value); }

	/// Converts any arithmetic type to int32 at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to int32.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr int32 ToI(T value) { return static_cast<int32>(value); }

	/// Returns max value of arithmetic type.
	/// @tparam T arithmetic type.
	template<typename T>
	requires std::is_arithmetic_v<T>
	inline constexpr T MaxVal() {
		return std::numeric_limits<T>::max();
	}

	/// @return Returns definitive size of vector in bytes.
	/// @param vector Vector to calculate size.
	template<typename T>
	inline constexpr usize ByteSize(const std::vector<T>& vector) {
		return sizeof(T) * vector.size();
	}

	/// @return Returns definitive size of type in bytes multiplied by size.
	/// @param arr Pointer to the data.
	/// @param size How much elements.
	template<typename T, usize L>
	inline constexpr usize ByteSize(const T (&arr)[L]) {
		return sizeof(T) * L;
	}

	/// @return Returns definitive size of type in bytes multiplied by size.
	/// @param arr Pointer to the data.
	/// @param size How much elements.
	template<typename T>
	inline constexpr usize ByteSize(const T* arr, usize size) {
		return sizeof(T) * size;
	}

	/// @return Underlying type of enum.
	/// @param e Enum.
	template<typename T>
		requires std::is_enum_v<T>
	inline constexpr std::underlying_type_t<T> ToUnderlyingEnum(T e){
		return static_cast<std::underlying_type_t<T>>(e);
	}

	/// @brief Hashes a UTF-8 string literal to a 64-bit unsigned integer.
	/// @tparam L Size of the string literal (including null terminator).
	/// @param str String literal to hash.
	/// @return 64-bit hash of the input string.
	template<usize L>
	inline constexpr uint64 HashStr(const char8 (&str)[L]) noexcept {
		return cstd::StringHasher::Hash(str);
	}
	inline uint64 HashStr(std::string_view str) noexcept {
		return cstd::StringHasher::Hash(str);
	}


	namespace settings {

		////////////////////////////////////////////////////////////////////////
		// ECS

		// Defines how much memory needs to be reserved for components
		LUM_COMPILE_VARIABLE usize ECS_MAX_POOL_CAPACITY	= 1024;
		LUM_COMPILE_VARIABLE usize ECS_MAX_ENTITY_COUNT	= 1024;

		// Defines how much containers needs to be reserved for components
		LUM_COMPILE_VARIABLE usize ECS_MAX_COMPONENT_TYPES_COUNT = 32;

		////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////
		// Audio

		// Defines how much memory needs to be reserved for audio
		LUM_COMPILE_VARIABLE usize AUDIO_MAX_SOUNDS_COUNT		= 256;
		LUM_COMPILE_VARIABLE usize AUDIO_MAX_CHANNELS_COUNT	= 256;

		////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////
		// Events

		// Defines how much memory needs to be reserved for pools

		LUM_COMPILE_VARIABLE usize EVENT_MAX_EVENT_TYPES = 32;

		// All settings under are for single event type

		LUM_COMPILE_VARIABLE usize EVENT_MAX_CALLBACKS_PER_FRAME	= 8;
		LUM_COMPILE_VARIABLE usize EVENT_MAX_PERMAMENT_CALLBACKS	= 8;
		LUM_COMPILE_VARIABLE usize EVENT_MAX_EMITTS_PER_FRAME		= 64;

		////////////////////////////////////////////////////////////////////////


	}
	// TODO change names below with -g
	namespace cmdcolor {
		LUM_COMPILE_VARIABLE ccharptr gReset	= "\033[0m";
		LUM_COMPILE_VARIABLE ccharptr gBlack	= "\033[30m";
		LUM_COMPILE_VARIABLE ccharptr gRed		= "\033[31m";
		LUM_COMPILE_VARIABLE ccharptr gGreen	= "\033[32m";
		LUM_COMPILE_VARIABLE ccharptr gYellow	= "\033[33m";
		LUM_COMPILE_VARIABLE ccharptr gBlue		= "\033[34m";
		LUM_COMPILE_VARIABLE ccharptr magenta	= "\033[35m";
		LUM_COMPILE_VARIABLE ccharptr cyan		= "\033[36m";
		LUM_COMPILE_VARIABLE ccharptr white		= "\033[37m";

		LUM_COMPILE_VARIABLE ccharptr light_black	= "\033[90m";
		LUM_COMPILE_VARIABLE ccharptr light_red		= "\033[91m";
		LUM_COMPILE_VARIABLE ccharptr light_green	= "\033[92m";
		LUM_COMPILE_VARIABLE ccharptr light_yellow	= "\033[93m";
		LUM_COMPILE_VARIABLE ccharptr light_blue	= "\033[94m";
		LUM_COMPILE_VARIABLE ccharptr light_magenta	= "\033[95m";
		LUM_COMPILE_VARIABLE ccharptr light_cyan	= "\033[96m";
		LUM_COMPILE_VARIABLE ccharptr light_white	= "\033[97m";

		LUM_COMPILE_VARIABLE ccharptr bg_black		= "\033[40m";
		LUM_COMPILE_VARIABLE ccharptr bg_red		= "\033[41m";
		LUM_COMPILE_VARIABLE ccharptr bg_green		= "\033[42m";
		LUM_COMPILE_VARIABLE ccharptr bg_yellow		= "\033[43m";
		LUM_COMPILE_VARIABLE ccharptr bg_blue		= "\033[44m";
		LUM_COMPILE_VARIABLE ccharptr bg_magenta	= "\033[45m";
		LUM_COMPILE_VARIABLE ccharptr bg_cyan		= "\033[46m";
		LUM_COMPILE_VARIABLE ccharptr bg_white		= "\033[47m";

		LUM_COMPILE_VARIABLE ccharptr bg_light_black	= "\033[100m";
		LUM_COMPILE_VARIABLE ccharptr bg_light_red		= "\033[101m";
		LUM_COMPILE_VARIABLE ccharptr bg_light_green	= "\033[102m";
		LUM_COMPILE_VARIABLE ccharptr bg_light_yellow	= "\033[103m";
		LUM_COMPILE_VARIABLE ccharptr bg_light_blue		= "\033[104m";
		LUM_COMPILE_VARIABLE ccharptr bg_light_magenta	= "\033[105m";
		LUM_COMPILE_VARIABLE ccharptr bg_light_cyan		= "\033[106m";
		LUM_COMPILE_VARIABLE ccharptr bg_light_white	= "\033[107m";

		LUM_COMPILE_VARIABLE ccharptr bold			= "\033[1m";
		LUM_COMPILE_VARIABLE ccharptr italic		= "\033[3m";
		LUM_COMPILE_VARIABLE ccharptr underline		= "\033[4m";
		LUM_COMPILE_VARIABLE ccharptr strikethrough	= "\033[9m";
		LUM_COMPILE_VARIABLE ccharptr reverse		= "\033[7m";
	}
}