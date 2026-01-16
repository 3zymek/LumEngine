#pragma once

#if defined(_MSC_VER)
#define LUM_FORCEINLINE __forceinline
#elif defined(__GNUC__)
#define LUM_FORCEINLINE __attribute__((always_inline)) inline
#else
#define LUM_FORCEINLINE inline
#endif

#define LUM_INLINE_QUALIFIER        inline
#define LUM_CONSTEXPR_QUALIFIER     constexpr
#define LUM_NODISCARD_QUALIFIER     [[nodiscard]]
#define LUM_FORCEINLINE_QUALIFIER   LUM_FORCEINLINE
#define LUM_NOEXCEPT_QUALIFIER      noexcept

// inline constexpr
#define LUM_CONST_VAR_QUALIFIER \
    LUM_INLINE_QUALIFIER \
    LUM_CONSTEXPR_QUALIFIER

// __forceinline constexpr
#define LUM_FUNC_HOT_QUALIFIER \
    LUM_FORCEINLINE_QUALIFIER \
    LUM_CONSTEXPR_QUALIFIER

// inline constexpr
#define LUM_FUNC_INLINE_QUALIFIER \
    LUM_INLINE_QUALIFIER \
    LUM_CONSTEXPR_QUALIFIER

// inline
#define LUM_FUNC_RUNTIME_QUALIFIER