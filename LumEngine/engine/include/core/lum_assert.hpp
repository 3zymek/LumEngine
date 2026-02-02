#pragma once
#include "logger.hpp"
#include "setup.hpp"

#if !defined(NDEBUG)
#   define LUM_ASSERT(expr, msg) \
        do { if(!(expr)) { LUM_LOG_FATAL(msg); LUM_DEBUGBREAK(); } } while(0)
#else
#   define LUM_ASSERT(expr, msg) ((void)0)
#endif

#if LUM_ENABLE_HOTPATH_VALIDATION == 1
#define LUM_HOTCHK(expr, msg) \
        do { if(!(expr)) { LUM_LOG_WARN(msg); } } while(0)

#define LUM_HOTCHK_RETURN_VOID(expr, msg) \
        do { if (!(expr)) { LUM_LOG_WARN(msg); return; } } while (0)

#define LUM_HOTCHK_RETURN_NPTR(expr, msg) \
        do { if (!(expr)) { LUM_LOG_WARN(msg); return nullptr; } } while (0)

#define LUM_HOTCHK_RETURN_0(expr, msg) \
        do { if (!(expr)) { LUM_LOG_WARN(msg); return 0; } } while (0)

#define LUM_HOTCHK_RETURN_CUSTOM(expr, msg, retival) \
        do { if(!(expr)) { LUM_LOG_WARN(msg); return retival; } } while(0)

#else
#define LUM_HOTCHK(expr, msg) ((void)0)

#define LUM_HOTCHK_RETURN_VOID(expr, msg) ((void)0)

#define LUM_HOTCHK_RETURN_NPTR(expr, msg) ((void)0)

#define LUM_HOTCHK_RETURN_0(expr, msg) ((void)0)

#define LUM_HOTCHK_RETURN_CUSTOM(expr, msg, retival) ((void)0)
#endif


#if LUM_ENABLE_LOGGER == 1
#define LUM_LOG_FATAL(msg) \
    do { lum::Logger::Get().log_cmd(lum::LogSeverity::FATAL, __FILE__, __func__, __LINE__, msg); } while (0)

#define LUM_LOG_ERROR(msg) \
    do { lum::Logger::Get().log_cmd(lum::LogSeverity::ERROR, __FILE__, __func__, __LINE__, msg); } while (0)

#define LUM_LOG_WARN(msg) \
    do { lum::Logger::Get().log_cmd(lum::LogSeverity::WARN, __FILE__, __func__, __LINE__, msg); } while (0)

#define LUM_LOG_INFO(msg, ...) \
    do { lum::Logger::Get().log_cmd(lum::LogSeverity::INFO, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)


#define LUM_LOG_DEBUG(msg) \
    do { lum::Logger::Get().log_cmd(lum::LogSeverity::DEBUG, __FILE__, __func__, __LINE__, msg); } while (0)



#else

#define LUM_LOG_FATAL(msg) ((void)0)

#define LUM_LOG_ERROR(msg) ((void)0)

#define LUM_LOG_WARN(msg) ((void)0)

#define LUM_LOG_INFO(msg) ((void)0)

#define LUM_LOG_DEBUG(msg) ((void)0)

#endif // ENABLE_LOGGER