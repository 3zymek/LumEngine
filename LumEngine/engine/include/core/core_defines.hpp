#pragma once

#if defined(_MSC_VER)
	#define LUM_FORCEINLINE __forceinline
#elif defined(__GNUC__)
	#define LUM_FORCEINLINE __attribute__((always_inline)) inline
#else
	#define LUM_FORCEINLINE inline
#endif

#if defined(__cplusplus) && __cplusplus >= 201703L
	#define LUM_NODISCARD [[nodiscard]]
#else
	#define LUM_NODISCARD
#endif

#define LUM_NOEXCEPT      noexcept

#define LUM_CONST_VAR_QUALIFIER inline constexpr