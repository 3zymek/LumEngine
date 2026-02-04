#pragma once
#include "logger.hpp"
#include "setup.hpp"

#if !defined(NDEBUG)
#   define LUM_ASSERT(expr, msg) \
        do { if(!(expr)) { LUM_LOG_FATAL(msg); LUM_DEBUGBREAK(); } } while(0)
#else
#   define LUM_ASSERT(expr, msg) ((void)0)
#endif

#define LUM_SASSERT(expr) \
    static_assert(expr)

#if LUM_ENABLE_HOTPATH_VALIDATION == 1
#define LUM_HOTCHK(expr, msg, ...) \
        do { if(!(expr)) { LUM_LOG_ERROR(msg, ##__VA_ARGS__); } } while(0)

#define LUM_HOTCHK_RETURN_VOID(expr, msg, ...) \
        do { if (!(expr)) { LUM_LOG_ERROR(msg, ##__VA_ARGS__); return; } } while (0)

#define LUM_HOTCHK_RETURN_NPTR(expr, msg, ...) \
        do { if (!(expr)) { LUM_LOG_ERROR(msg, ##__VA_ARGS__); return nullptr; } } while (0)

#define LUM_HOTCHK_RETURN_0(expr, msg, ...) \
        do { if (!(expr)) { LUM_LOG_ERROR(msg, ##__VA_ARGS__); return 0; } } while (0)

#define LUM_HOTCHK_RETURN_CUSTOM(expr, retival, msg, ...) \
        do { if(!(expr)) { LUM_LOG_ERROR(msg, ##__VA_ARGS__); return retival; } } while(0)

#else
#define LUM_HOTCHK(expr, msg, ...) ((void)0)

#define LUM_HOTCHK_RETURN_VOID(expr, msg, ...) ((void)0)

#define LUM_HOTCHK_RETURN_NPTR(expr, msg, ...) ((void)0)

#define LUM_HOTCHK_RETURN_0(expr, msg, ...) ((void)0)

#define LUM_HOTCHK_RETURN_CUSTOM(expr, retival, msg, ...) ((void)0)
#endif


#if LUM_ENABLE_LOGGER == 1
#define LUM_LOG_FATAL(msg, ...) \
    do { lum::Logger::Get().log_cmd(lum::LogSeverity::Fatal, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); LUM_DEBUGBREAK(); } while (0)

#define LUM_LOG_ERROR(msg, ...) \
    do { lum::Logger::Get().log_cmd(lum::LogSeverity::Error, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

#define LUM_LOG_WARN(msg, ...) \
    do { lum::Logger::Get().log_cmd(lum::LogSeverity::Warn, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

#define LUM_LOG_INFO(msg, ...) \
    do { lum::Logger::Get().log_cmd(lum::LogSeverity::Info, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)


#define LUM_LOG_DEBUG(msg, ...) \
    do { lum::Logger::Get().log_cmd(lum::LogSeverity::Debug, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)



#else

#define LUM_LOG_FATAL(msg) ((void)0)

#define LUM_LOG_ERROR(msg) ((void)0)

#define LUM_LOG_WARN(msg) ((void)0)

#define LUM_LOG_INFO(msg) ((void)0)

#define LUM_LOG_DEBUG(msg) ((void)0)

#endif // ENABLE_LOGGER