//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core helpers and utilities for the LumEngine.
//
//=============================================================================//
#pragma once

#include "Core/CorePch.hpp"
#include "Core/CoreDefines.hpp"
#include "Core/Types.hpp"
#include "Core/Limits.hpp"

#include "Core/Math/MathCommon.hpp"

#include "Core/Utils/StringHasher.hpp"
#include "Core/Utils/Logger.hpp"
#include "Core/Utils/LumAssert.hpp"
#include "Core/Utils/FormatString.hpp"
#include "Core/Utils/SafePtr.hpp"

namespace lum {

	/* @brief Generates a unique numeric ID for a given type.
	* Each instantiation maintains its own atomic counter starting at tNullVal + 1.
	* @tparam tType  Type to generate IDs for.
	* @tparam tNullVal Sentinel null value; the counter starts one above this.
	*/
	template<typename tType, std::integral tIntegral = usize, tIntegral tNullVal = 0>
	struct LUM_API UniqueID {

		/* @brief Returns the next available unique ID.
		* @return Atomically incremented uint32 identifier.
		*/
		static tIntegral Get( ) {
			return s_Id.fetch_add( 1, std::memory_order_relaxed );
		}

	private:

		/* @brief Shared atomic counter for this type instantiation. */
		inline static std::atomic<tIntegral> s_Id = tNullVal + 1;

	};



	/* @brief Generates a unique compile-time stable ID per type.
	* Each distinct type receives a single ID assigned on first call.
	*/
	struct LUM_API TypeRegistry {

		/* @brief Returns the unique ID assigned to tType.
		* @tparam tType Type to retrieve the ID for.
		* @return Stable uint32 identifier for the given type.
		*/
		template<typename tType>
		static uint32 GetTypeId( ) {
			static uint32 id = s_Id.fetch_add( 1, std::memory_order_relaxed );
			return id;
		}

	private:

		/* @brief Global atomic counter shared across all type ID assignments. */
		inline static std::atomic<uint32> s_Id = 0;

	};

	/* @brief Converts any arithmetic type to String at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to String.
	*/
	template<cArithmetic tType>
	inline constexpr String ToString( tType value ) { 
		return std::to_string( value ); 
	}

	/* @brief Returns the maximum representable value for an arithmetic type.
	* @tparam T Arithmetic type to query.
	* @return std::numeric_limits<tType>::max().
	*/
	template<cArithmetic tType>
	inline constexpr tType MaxValue( ) { 
		return std::numeric_limits<tType>::max( ); 
	}

	/* @brief Returns the minimum representable value for an arithmetic type.
	* @tparam T Arithmetic type to query.
	* @return std::numeric_limits<tType>::min().
	*/
	template<cArithmetic tType>
	inline constexpr tType MinValue( ) { 
		return std::numeric_limits<tType>::lowest( ); 
	}

	/* @brief Returns the total byte size of a std::vector's contents.
	* @param vector Vector to calculate byte size for.
	* @return sizeof(T) * vector.size().
	*/
	template<typename tType>
	inline constexpr usize ComputeByteSize( const std::vector<tType>& vector ) {
		return sizeof( tType ) * vector.size( );
	}

	/* @brief Returns the total byte size of a raw array.
	* @param arr Reference to the raw array.
	* @return sizeof(T) * L.
	*/
	template<typename tType, usize tL>
	inline constexpr usize ComputeByteSize( const tType( &arr )[ tL ] ) {
		return sizeof( tType ) * tL;
	}

	/* @brief Returns the total byte size of a pointer-based array.
	* @param arr  Pointer to the data.
	* @param size Number of elements.
	* @return sizeof(T) * size.
	*/
	template<typename tType>
	inline constexpr usize ComputeByteSize( const tType* arr, usize size ) {
		return sizeof( tType ) * size;
	}

	/* @brief Returns the element count of a raw array.
	* @param arr Reference to the raw array.
	* @return Number of elements L.
	*/
	template<typename tType, usize tL>
	inline constexpr usize ArraySize( const tType( &arr )[ tL ] ) {
		return tL;
	}

	/* @brief Casts an enum value to its underlying integral type.
	* @param e Enum value to convert.
	* @return The underlying integer value of the enum.
	*/
	template<typename tEnum>
		requires std::is_enum_v<tEnum>
	inline constexpr std::underlying_type_t<tEnum> ToUnderlyingEnum( tEnum e ) {
		return static_cast< std::underlying_type_t<tEnum> >(e);
	}

	/* @brief Hashes a UTF-8 string literal to a 64-bit unsigned integer at compile-time.
	* @tparam L Size of the string literal including null terminator.
	* @param str String literal to hash.
	* @return 64-bit hash of the input string.
	*/
	template<usize tL>
	inline constexpr uint64 HashString( const char( &str )[ tL ] ) noexcept {
		return cstd::StringHasher::Hash( str );
	}

	/* @brief Hashes a StringView to a 64-bit unsigned integer at runtime.
	* @param str String view to hash.
	* @return 64-bit hash of the input string.
	*/
	inline constexpr uint64 HashString( StringView str ) noexcept {
		return cstd::StringHasher::Hash( str );
	}

	/* @brief Converts a String to its lowercase equivalent.
	* @param str Input string to convert.
	* @return New string with all characters lowercased.
	*/
	template<cStringable tType>
	inline String ToLower( tType str ) {
		String result = str.data( );
		std::transform( result.begin( ), result.end( ), result.begin( ), ::tolower );
		return result;
	}

	template<>
	inline String ToLower( char c ) {
		return String( 1, tolower( c ) );
	}

	/* @brief Converts a String to its uppercase equivalent.
	* @param str Input string to convert.
	* @return New string with all characters upeercased.
	*/
	inline String ToUpper( StringView str ) {
		String result = str.data( );
		std::transform( result.begin( ), result.end( ), result.begin( ), ::toupper );
		return result;
	}

	template<cArithmetic tTarget, cArithmetic tSource>
	LUM_NODISCARD inline constexpr tTarget SafeCast( tSource value ) noexcept {
#		if LUM_DEBUG
			if constexpr (std::is_integral_v<tSource> && std::is_integral_v<tTarget>) {
				LUM_ASSERT(
					std::in_range<tTarget>( value ),
					"SafeCast failed: Integer value out of range for target type!"
				);
			}
			else if constexpr (std::is_floating_point_v<tSource> && std::is_integral_v<tTarget>) {
				LUM_ASSERT(
					std::isfinite( value ) &&
					value >= static_cast<tSource>(std::numeric_limits<tTarget>::lowest( )) &&
					value <= static_cast<tSource>(std::numeric_limits<tTarget>::max( )),
					"SafeCast failed: Floating-point value out of range for target type."
				);
			}
			else if constexpr (std::is_integral_v<tSource> && std::is_floating_point_v<tTarget>) {
				LUM_ASSERT(
					static_cast<long double>(value) >= std::numeric_limits<tTarget>::lowest( ) &&
					static_cast<long double>(value) <= std::numeric_limits<tTarget>::max( ),
					"SafeCast failed: Integer value out of range for target type."
				);
			}
			else {
				LUM_ASSERT(
					std::isfinite( value ) &&
					value >= std::numeric_limits<tTarget>::lowest( ) &&
					value <= std::numeric_limits<tTarget>::max( ),
					"SafeCast failed: Floating-point value out of range for target type."
				);
			}
#		endif

		return static_cast<tTarget>(value);
	}

} // namespace lum