#pragma once

#if defined(_MSC_VER)
#define LUM_FORCEINLINE __forceinline
#define LUM_DEBUGBREAK() __debugbreak()

#elif defined(__GNUC__)
#	define LUM_FORCEINLINE __attribute__((always_inline)) inline
#	define LUM_DEBUGBREAK() __builtin_trap()

#elif defined(__clang__)
#	define LUM_FORCEINLINE __attribute__((always_inline)) inline
#	define LUM_DEBUGBREAK() __builtin_debugtrap()

#else
#	define LUM_FORCEINLINE inline
#	define LUM_DEBUGBREAK() ((void)0)
#endif

#if (defined(__cplusplus) && __cplusplus >= 201703L) \
    || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#   define LUM_NODISCARD [[nodiscard]]
#else
#   define LUM_NODISCARD
#endif

#define LUM_NOEXCEPT      noexcept

#define LUM_CONST_VAR_QUALIFIER inline constexpr