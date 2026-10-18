#pragma once

#include <limits>
#include <type_traits>
#include <utility>

#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Utils/Assert.hpp"

namespace lum {

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

} // namespace  lum 