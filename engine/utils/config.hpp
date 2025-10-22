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
#ifdef DEBUG_ENGINE
	#define LOG_ENGINE(x) std::cout << "[ENGINE] " << x << '\n';
#else
	#define LOG_ENGINE(x)
#endif // DEBUG_ENGINE
