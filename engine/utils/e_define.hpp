#pragma once	
#include <iostream>
#define NODISCARD		[[nodiscard]]
#define NORETURN		[[noreturn]]
#define LIKELEY			[[likely]]
#define MAYBE_UNUSED	[[maybe_unused]]

#ifdef _WIN32
	#define FORCEINLINE		__forceinline
	#define ABSTRACT_CLASS	class __declspec(novtable)
#else
	#define FORCEINLINE		inline
	#define ABSTRACT_CLASS	class
#endif // _WIN32


#define DEBUG_ENGINE
#define DEBUG_EVENT

#ifdef DEBUG_ENGINE
	#define ENGINE_LOG(x) \
		do { std::cout << "[ENGINE] " << x << '\n'; } while(0);
#else
	#define LOG_ENGINE(x)
#endif // DEBUG_ENGINE
