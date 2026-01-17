#pragma once

#if defined(_MSC_VER)
#define LUM_FORCEINLINE __forceinline
#elif defined(__GNUC__)
#define LUM_FORCEINLINE __attribute__((always_inline)) inline
#else
#define LUM_FORCEINLINE inline
#endif

#define LUM_NODISCARD     [[nodiscard]]
#define LUM_NOEXCEPT      noexcept

#define LUM_CONST_VAR_QUALIFIER inline constexpr