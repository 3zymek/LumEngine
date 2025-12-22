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

#ifdef ENABLE_LOGGER

#define LOG_FATAL(msg) \
    do { Logger::Get().Log(LogSeverity::FATAL, __FILE__, __func__, __LINE__, msg); } while (0)

#define LOG_ERROR(msg) \
    do { Logger::Get().Log(LogSeverity::ERROR, __FILE__, __func__, __LINE__, msg); } while (0)

#define LOG_ERROR_S(msg) \
    do { Logger::Get().Log(LogSeverity::ERROR, __FILE__, __func__, __LINE__, msg.c_str()); } while (0)

#define LOG_WARN(msg) \
    do { Logger::Get().Log(LogSeverity::WARN, __FILE__, __func__, __LINE__, msg); } while (0)

#define LOG_WARN_S(msg) \
    do { Logger::Get().Log(LogSeverity::WARN, __FILE__, __func__, __LINE__, msg.c_str()); } while (0)

#define LOG_INFO(msg) \
    do { Logger::Get().Log(LogSeverity::INFO, __FILE__, __func__, __LINE__, msg); } while (0)

#define LOG_INFO_S(msg) \
    do { Logger::Get().Log(LogSeverity::INFO, __FILE__, __func__, __LINE__, msg.c_str()); } while (0)

#define LOG_DEBUG(msg) \
    do { Logger::Get().Log(LogSeverity::DEBUG, __FILE__, __func__, __LINE__, msg); } while (0)

#define LOG_DEBUG_S(msg) \
    do { Logger::Get().Log(LogSeverity::DEBUG, __FILE__, __func__, __LINE__, msg.c_str()); } while (0)

#define LOG_TRACE(msg) \
    do { Logger::Get().Log(LogSeverity::TRACE, __FILE__, __func__, __LINE__, msg); } while (0)

#define LOG_TRACE_S(msg) \
    do { Logger::Get().Log(LogSeverity::TRACE, __FILE__, __func__, __LINE__, msg.c_str()); } while (0)

#define LOG_INIT_OK(msg) \
    do { Logger::Get().LogInit(InitStatus::OK, msg); } while (0)

#define LOG_INIT_FAIL(msg) \
    do { Logger::Get().LogInit(InitStatus::FAIL, msg); } while (0)


#else

#define LOG_FATAL(msg) ((void)0)
#define LOG_FATAL_S(msg) ((void)0)

#define LOG_ERROR(msg) ((void)0)
#define LOG_ERROR_S(msg) ((void)0)

#define LOG_WARN(msg) ((void)0)
#define LOG_WARN_S(msg) ((void)0)

#define LOG_INFO(msg) ((void)0)
#define LOG_INFO_S(msg) ((void)0)

#define LOG_DEBUG(msg) ((void)0)
#define LOG_DEBUG_S(msg) ((void)0)

#define LOG_TRACE(msg) ((void)0)
#define LOG_TRACE_S(msg) ((void)0)

#define LOG_INIT_OK(msg) ((void)0)

#define LOG_INIT_FAIL(msg) ((void)0)

#endif // ENABLE_LOGGER


#define EV_ASSERT_IS_EVENT(T) \
			static_assert(std::is_base_of_v<BaseEvent, T> and !std::is_same_v<BaseEvent, T>, "Event must be derived from BaseEvent")