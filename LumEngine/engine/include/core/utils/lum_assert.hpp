//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Logging system and assertion macros
//
//=============================================================================//
#pragma once

#include "core/setup.hpp"

#if !defined(NDEBUG)
#   define LUM_ASSERT(expr, msg) \
        do { if(!(expr)) { LUM_LOG_FATAL(msg); LUM_DEBUGBREAK(); } } while(0)
#else
#   define LUM_ASSERT(expr, msg) ((void)0)
#endif

#define LUM_SASSERT(expr) \
    static_assert(expr)

#if LUM_ENABLE_HOTPATH_VALIDATION == 1
#   define LUM_HOTCHK(expr, sev, msg, ...) \
        do { if(!(expr)) { LUM_LOG(msg, sev, ##__VA_ARGS__); } } while(0)

#   define LUM_HOTCHK_RETURN_VOID(expr, sev, msg, ...) \
        do { if (!(expr)) { LUM_LOG(msg, sev, ##__VA_ARGS__); return; } } while (0)

#   define LUM_HOTCHK_RETURN_NPTR(expr, sev, msg, ...) \
        do { if (!(expr)) { LUM_LOG(msg, sev, ##__VA_ARGS__); return nullptr; } } while (0)

#   define LUM_HOTCHK_RETURN_0(expr, sev, msg, ...) \
        do { if (!(expr)) { LUM_LOG(msg, sev, ##__VA_ARGS__); return 0; } } while (0)

#   define LUM_HOTCHK_RETURN_CUSTOM(expr, sev, retival, msg, ...) \
        do { if(!(expr)) { LUM_LOG(msg, sev, ##__VA_ARGS__); return retival; } } while(0)

#else
#   define LUM_HOTCHK(expr, sev, msg, ...) ((void)0)

#   define LUM_HOTCHK_RETURN_VOID(expr, sev, msg, ...) ((void)0)

#   define LUM_HOTCHK_RETURN_NPTR(expr, sev, msg, ...) ((void)0)

#   define LUM_HOTCHK_RETURN_0(expr, sev, msg, ...) ((void)0)

#   define LUM_HOTCHK_RETURN_CUSTOM(expr, sev, retival, msg, ...) ((void)0)
#endif


#if LUM_ENABLE_LOGGER == 1

#   define LUM_SEV_DEBUG LogSeverity::Debug
#   define LUM_SEV_INFO  LogSeverity::Info
#   define LUM_SEV_WARN  LogSeverity::Warn
#   define LUM_SEV_ERROR LogSeverity::Error
#   define LUM_SEV_FATAL LogSeverity::Fatal

#   define LUM_LOG_FATAL(msg, ...) \
        do { lum::Logger::Get().LogCmd(lum::LogSeverity::Fatal, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

#   define LUM_LOG_ERROR(msg, ...) \
        do { lum::Logger::Get().LogCmd(lum::LogSeverity::Error, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

#   define LUM_LOG_WARN(msg, ...) \
        do { lum::Logger::Get().LogCmd(lum::LogSeverity::Warn, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

#   define LUM_LOG_INFO(msg, ...) \
        do { lum::Logger::Get().LogCmd(lum::LogSeverity::Info, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

#   define LUM_LOG_DEBUG(msg, ...) \
        do { lum::Logger::Get().LogCmd(lum::LogSeverity::Debug, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while (0)

#   define LUM_LOG(msg, sev, ...) \
        do { lum::Logger::Get().LogCmd(sev, __FILE__, __func__, __LINE__, msg, ##__VA_ARGS__); } while(0)



#else

#   define LUM_SEV_DEBUG ((void)0)
#   define LUM_SEV_INFO  ((void)0)
#   define LUM_SEV_WARN  ((void)0)
#   define LUM_SEV_ERROR ((void)0)
#   define LUM_SEV_FATAL ((void)0)

#   define LUM_LOG_FATAL(msg, ...) ((void)0)

#   define LUM_LOG_ERROR(msg, ...) ((void)0)

#   define LUM_LOG_WARN(msg, ...) ((void)0)

#   define LUM_LOG_INFO(msg, ...) ((void)0)

#   define LUM_LOG_DEBUG(msg, ...) ((void)0)

#   define LUM_LOG(msg, sev, ...) ((void)0)

#endif // ENABLE_LOGGER