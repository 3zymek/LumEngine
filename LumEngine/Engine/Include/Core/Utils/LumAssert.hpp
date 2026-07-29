//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Logging system and assertion macros
//
//=============================================================================//
#pragma once

#include "Core/Setup.hpp"

#if !defined(NDEBUG)
#	define LUM_ASSERT(expr, msg, ...) \
		do { if(!(expr)) { LUM_LOG_FATAL(msg, ##__VA_ARGS__); LUM_DEBUGBREAK(); } } while(0)
#else
#	define LUM_ASSERT(expr, msg, ...) ((void)0)
#endif

	/* @brief Compile-time static assertion check.
	*  @param expr Constant expression to evaluate at compile time.
	*/
#define LUM_SASSERT(expr) \
	static_assert(expr)

	/* @brief Logs a message and returns from the current function if the condition is true.
	*  @param expr Condition to check.
	*  @param sev Severity level of the log message.
	*  @param msg Formatted log message.
	*/
#define LUM_RETURN_IF(expr, sev, msg, ...) \
	do { if((expr)) { LUM_LOG(msg, sev, ##__VA_ARGS__); return; } } while(0)

	/* @brief Logs a message and returns a default-constructed object if the condition is true.
	*  @param expr Condition to check.
	*  @param sev Severity level of the log message.
	*  @param msg Formatted log message.
	*/
#define LUM_RETURN_DEF_IF(expr, sev, msg, ...) \
	do { if((expr)) { LUM_LOG(msg, sev, ##__VA_ARGS__); return {}; } } while(0)


#if LUM_ENABLE_LOGGER == 1

	/* @brief Debug severity level alias. */
#	define LUM_SEV_DEBUG LogSeverityLevel::Debug

	/* @brief Info severity level alias. */
#	define LUM_SEV_INFO  LogSeverityLevel::Info

	/* @brief Warning severity level alias. */
#	define LUM_SEV_WARN  LogSeverityLevel::Warn

	/* @brief Error severity level alias. */
#	define LUM_SEV_ERROR LogSeverityLevel::Error

	/* @brief Fatal severity level alias. */
#	define LUM_SEV_FATAL LogSeverityLevel::Fatal

	/* @brief Logs a critical, unrecoverable failure message.
	*  @param msg Formatted log message.
	*/
#	define LUM_LOG_FATAL(msg, ...) \
		do { lum::Logger::Get().LogCmd(lum::LogSeverityLevel::Fatal, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

	/* @brief Logs a recoverable error message.
	*  @param msg Formatted log message.
	*/
#	define LUM_LOG_ERROR(msg, ...) \
		do { lum::Logger::Get().LogCmd(lum::LogSeverityLevel::Error, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

	/* @brief Logs a warning message.
	*  @param msg Formatted log message.
	*/
#	define LUM_LOG_WARN(msg, ...) \
		do { lum::Logger::Get().LogCmd(lum::LogSeverityLevel::Warn, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

	/* @brief Logs a general information message.
	*  @param msg Formatted log message.
	*/
#	define LUM_LOG_INFO(msg, ...) \
		do { lum::Logger::Get().LogCmd(lum::LogSeverityLevel::Info, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

	/* @brief Logs a debug-only message.
	*  @param msg Formatted log message.
	*/
#	define LUM_LOG_DEBUG(msg, ...) \
		do { lum::Logger::Get().LogCmd(lum::LogSeverityLevel::Debug, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

	/* @brief Logs a message with a custom severity level.
	*  @param msg Formatted log message.
	*  @param sev Severity level.
	*/
#	define LUM_LOG(msg, sev, ...) \
		do { lum::Logger::Get().LogCmd(sev, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while(0)

#else

#	define LUM_SEV_DEBUG ((void)0)
#	define LUM_SEV_INFO  ((void)0)
#	define LUM_SEV_WARN  ((void)0)
#	define LUM_SEV_ERROR ((void)0)
#	define LUM_SEV_FATAL ((void)0)
#	define LUM_LOG_FATAL(msg, ...) ((void)0)
#	define LUM_LOG_ERROR(msg, ...) ((void)0)
#	define LUM_LOG_WARN(msg, ...) ((void)0)
#	define LUM_LOG_INFO(msg, ...) ((void)0)
#	define LUM_LOG_DEBUG(msg, ...) ((void)0)
#	define LUM_LOG(msg, sev, ...) ((void)0)

#endif // LUM_ENABLE_LOGGER