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


#define DEBUG_ENGINE
#define DEBUG_EVENT

#ifdef DEBUG_ENGINE
	#define ENGINE_LOG(x) \
		do { std::cout << "[ENGINE] " << x << '\n'; } while(0);
#else
	#define LOG_ENGINE(x)
#endif // DEBUG_ENGINE