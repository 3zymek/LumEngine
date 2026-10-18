#pragma once

#include <limits>
#include <span>
#include <type_traits>
#include <vector>

#include <MagicEnum/magic_enum.hpp>

#include "Core/CoreTypes.hpp"

namespace lum {

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
	inline constexpr usize ComputeByteSize( std::span<tType> arr ) {
		return arr.size();
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
	template<cEnum tEnum>
	inline constexpr std::underlying_type_t<tEnum> ToUnderlyingEnum( tEnum e ) {
		return static_cast<std::underlying_type_t<tEnum>>(e);
	}

	template<cEnum tEnum>
	inline constexpr usize t_EnumCount = magic_enum::enum_count<tEnum>( );

	//template<cEnum tEnum>
	//inline constexpr StringView t_EnumName = magic_enum::enum_name<tEnum>( );

} // namespace lum