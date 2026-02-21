//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Core helpers in LumEngine
//
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"
#include "core/types.hpp"
#include "core/utils/string_hasher.hpp"
#include "core/math/types.hpp"

namespace lum {



	/// Generates ID for any type
	/// @tparam T object Type to generate an ID
	/// @tparam tNullVal Null value for ID's
	template<typename tType, int32 tNullVal = 0>
	struct GenerateID {
		static uint32 Get() {
			return gID.fetch_add(1, std::memory_order_relaxed);
		}
	private:
		inline static std::atomic<uint32> gID = tNullVal + 1;
	};

	struct GenerateTypeID {
		template<typename tType>
		static uint32 Get() {
			static uint32 id = gID.fetch_add(1, std::memory_order_relaxed);
			return id;
		}
	private:
		inline static std::atomic<uint32> gID = 0;
	};

	
	/// Converts any arithmetic type to float at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to float.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr float32 ToFloat32(T value) { return static_cast<float32>(value); }



	/// Converts any arithmetic type to uint8 at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint8.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint8 ToUint8(T value) { return static_cast<uint8>(value); }



	/// Converts any arithmetic type to uint16 at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint16.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint16 ToUint16(T value) { return static_cast<uint16>(value); }



	/// Converts any arithmetic type to uint32_t at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint32_t.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint32 ToUint32(T value) { return static_cast<uint32>(value); }



	/// Converts any arithmetic type to uint64_t at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to uint64_t.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr uint64_t ToUint64(T value) { return static_cast<uint64_t>(value); }



	/// Converts any arithmetic type to double at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to double.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr float64 ToFloat64(T value) { return static_cast<float64>(value); }



	/// Converts any arithmetic type to int32 at compile-time.
	/// @param value The numeric value to cast.
	/// @return The value casted to int32.
	template<typename T>
		requires std::is_arithmetic_v<T>
	inline constexpr int32 ToInteger(T value) { return static_cast<int32>(value); }



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



	/// @return Returns how much elements has raw array
	/// @param arr Array
	template<typename T, usize L>
	inline constexpr usize ArraySize(const T(&arr)[L]) {
		return L;
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

	namespace cmdcolor {
	
		LUM_COMPILE_VARIABLE ccharptr Reset		= "\033[0m";
		LUM_COMPILE_VARIABLE ccharptr Black		= "\033[30m";
		LUM_COMPILE_VARIABLE ccharptr Red		= "\033[31m";
		LUM_COMPILE_VARIABLE ccharptr Green		= "\033[32m";
		LUM_COMPILE_VARIABLE ccharptr Yellow	= "\033[33m";
		LUM_COMPILE_VARIABLE ccharptr Blue		= "\033[34m";
		LUM_COMPILE_VARIABLE ccharptr Magenta	= "\033[35m";
		LUM_COMPILE_VARIABLE ccharptr Cyan		= "\033[36m";
		LUM_COMPILE_VARIABLE ccharptr White		= "\033[37m";

		LUM_COMPILE_VARIABLE ccharptr LightBlack	= "\033[90m";
		LUM_COMPILE_VARIABLE ccharptr LightRed		= "\033[91m";
		LUM_COMPILE_VARIABLE ccharptr LightGreen	= "\033[92m";
		LUM_COMPILE_VARIABLE ccharptr LightYellow	= "\033[93m";
		LUM_COMPILE_VARIABLE ccharptr LightBlue		= "\033[94m";
		LUM_COMPILE_VARIABLE ccharptr LightMagenta	= "\033[95m";
		LUM_COMPILE_VARIABLE ccharptr LightCyan		= "\033[96m";
		LUM_COMPILE_VARIABLE ccharptr LightWhite	= "\033[97m";

		LUM_COMPILE_VARIABLE ccharptr BgBlack		= "\033[40m";
		LUM_COMPILE_VARIABLE ccharptr BgRed			= "\033[41m";
		LUM_COMPILE_VARIABLE ccharptr BgGreen		= "\033[42m";
		LUM_COMPILE_VARIABLE ccharptr BgYellow		= "\033[43m";
		LUM_COMPILE_VARIABLE ccharptr BgBlue		= "\033[44m";
		LUM_COMPILE_VARIABLE ccharptr BgMagenta		= "\033[45m";
		LUM_COMPILE_VARIABLE ccharptr BgCyan		= "\033[46m";
		LUM_COMPILE_VARIABLE ccharptr BgWhite		= "\033[47m";

		LUM_COMPILE_VARIABLE ccharptr BgLightBlack		= "\033[100m";
		LUM_COMPILE_VARIABLE ccharptr BgLightRed		= "\033[101m";
		LUM_COMPILE_VARIABLE ccharptr BgLightGreen		= "\033[102m";
		LUM_COMPILE_VARIABLE ccharptr BgLightYellow		= "\033[103m";
		LUM_COMPILE_VARIABLE ccharptr BgLightBlue		= "\033[104m";
		LUM_COMPILE_VARIABLE ccharptr BgLightMagenta	= "\033[105m";
		LUM_COMPILE_VARIABLE ccharptr BgLightCyan		= "\033[106m";
		LUM_COMPILE_VARIABLE ccharptr BgLightWhite		= "\033[107m";

		LUM_COMPILE_VARIABLE ccharptr FontBold			= "\033[1m";
		LUM_COMPILE_VARIABLE ccharptr FontItalic		= "\033[3m";
		LUM_COMPILE_VARIABLE ccharptr FontUnderline		= "\033[4m";
		LUM_COMPILE_VARIABLE ccharptr FontStrikethrough	= "\033[9m";
		LUM_COMPILE_VARIABLE ccharptr FontReserve		= "\033[7m";
	
	}
}