//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core defines for multi-platform code
// 
//=============================================================================//
#pragma once

#define LPROPERTY(...)
#define LFUNCTION(...)
#define LCLASS(...)
#define LUM_GENERATED_BODY( )
#define LUM_CLASS_EXTENSIONS( )

#if defined(_WIN32)
#   if defined(LUM_ENGINE_BUILD)
#       define LUM_API __declspec(dllexport)
#   elif defined(LUM_DYNAMIC_LIBRARIES)
#       define LUM_API __declspec(dllimport)
#   else
#       define LUM_API
#   endif
#else
#   define LUM_API
#endif

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