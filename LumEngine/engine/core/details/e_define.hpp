#pragma once	
#include <iostream>
#include <filesystem>
#include <string>

#define static_class class

#ifdef _WIN32
	#define force_inline		__forceinline
	#define abstract_class		class __declspec(novtable)

#else
	#define force_inline		inline
	#define abstract_class		class
#endif // _WIN32


#define ERRORS

#define DEBUG_ENGINE
//#define DEBUG_EVENT
//#define DEBUG_AUDIO
//#define DEBUG_ECS

#ifdef ERRORS
	#define LOG_ERROR(msg) \
		do { std::cout << "[ERROR] " << msg << '\n'; } while(0);
#else
	#define LOG_ERROR(x)
#endif //LOG_ERROR


#ifdef DEBUG_ENGINE
	#define ENGINE_LOG(x) \
		do { std::cout << "[ENGINE] " << x << '\n'; } while(0);
#else
	#define LOG_ENGINE(x)
#endif // DEBUG_ENGINE