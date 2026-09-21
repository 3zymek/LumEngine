#pragma once
#include "Core/Types.hpp"
#include "Core/Setup.hpp"

namespace lum {
	enum class LogSeverityLevel : uint8;
	class Logger;
}


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
		do { lum::Logger::Get().LogCmd( lum::LogSeverityLevel::Fatal, std::source_location::current( ), msg, ##__VA_ARGS__ ); } while (0)

	/* @brief Logs a recoverable error message.
	*  @param msg Formatted log message.
	*/
#	define LUM_LOG_ERROR(msg, ...) \
		do { lum::Logger::Get().LogCmd( lum::LogSeverityLevel::Error, std::source_location::current( ), msg, ##__VA_ARGS__ ); } while (0)

	/* @brief Logs a warning message.
	*  @param msg Formatted log message.
	*/
#	define LUM_LOG_WARN(msg, ...) \
		do { lum::Logger::Get().LogCmd( lum::LogSeverityLevel::Warn, std::source_location::current( ), msg, ##__VA_ARGS__ ); } while (0)

	/* @brief Logs a general information message.
	*  @param msg Formatted log message.
	*/
#	define LUM_LOG_INFO(msg, ...) \
		do { lum::Logger::Get().LogCmd( lum::LogSeverityLevel::Info, std::source_location::current( ), msg, ##__VA_ARGS__ ); } while (0)

	/* @brief Logs a debug-only message.
	*  @param msg Formatted log message.
	*/
#	define LUM_LOG_DEBUG(msg, ...) \
		do { lum::Logger::Get().LogCmd( lum::LogSeverityLevel::Debug, std::source_location::current( ), msg, ##__VA_ARGS__ ); } while (0)

	/* @brief Logs a message with a custom severity level.
	*  @param msg Formatted log message.
	*  @param sev Severity level.
	*/
#	define LUM_LOG(msg, sev, ...) \
		do { lum::Logger::Get().LogCmd( sev, std::source_location::current( ), msg, ##__VA_ARGS__ ); } while(0)

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