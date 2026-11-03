#pragma once

#include "Core/CoreDefines.hpp"
#include "Core/Utils/LogMacros.hpp"

#if !defined(NDEBUG)
#	define LUM_ASSERT(expr, msg, ...) \
		do { if(!(expr)) { LUM_LOG_FATAL( msg, ##__VA_ARGS__ ); LUM_DEBUGBREAK(); } } while(0)
#else
#	define LUM_ASSERT(expr, msg, ...) ((void)0)
#endif

/* @brief Compile-time static assertion check.
*  @param expr Constant expression to evaluate at compile time.
*/
#define LUM_SASSERT(expr) \
	static_assert(expr)