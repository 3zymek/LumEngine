#include "logger.hpp"
#define static_class class

#ifdef _WIN32
#define force_inline		__forceinline
#define abstract_class		class __declspec(novtable)

#else
#define force_inline		inline
#define abstract_class		class
#endif // _WIN32

#define ENABLE_LOGGER
#define DEBUG_RENDER

#ifdef ENABLE_LOGGER
#define LOG_FATAL(msg) \
    do { lum::Logger::Get().Log(lum::LogSeverity::FATAL, __FILE__, __func__, __LINE__, msg); } while (0)

#define LOG_ERROR(msg) \
    do { lum::Logger::Get().Log(lum::LogSeverity::ERROR, __FILE__, __func__, __LINE__, msg); } while (0)

#define LOG_WARN(msg) \
    do { lum::Logger::Get().Log(lum::LogSeverity::WARN, __FILE__, __func__, __LINE__, msg); } while (0)

#define LOG_INFO(msg) \
    do { lum::Logger::Get().Log(lum::LogSeverity::INFO, __FILE__, __func__, __LINE__, msg); } while (0)


#define LOG_DEBUG(msg) \
    do { lum::Logger::Get().Log(lum::LogSeverity::DEBUG, __FILE__, __func__, __LINE__, msg); } while (0)


#define LOG_INIT_OK(msg) \
    do { lum::Logger::Get().LogInit(lum::InitStatus::OK, msg); } while (0)

#define LOG_INIT_FAIL(msg) \
    do { lum::Logger::Get().LogInit(lum::InitStatus::FAIL, msg); } while (0)


#else

#define LOG_FATAL(msg) ((void)0)

#define LOG_ERROR(msg) ((void)0)

#define LOG_WARN(msg) ((void)0)

#define LOG_INFO(msg) ((void)0)

#define LOG_DEBUG(msg) ((void)0)

#define LOG_INIT_OK(msg) ((void)0)

#define LOG_INIT_FAIL(msg) ((void)0)

#endif // ENABLE_LOGGER