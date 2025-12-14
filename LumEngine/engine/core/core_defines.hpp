#pragma once
#include <cstdlib>
#include <iostream>

#define static_class class

#ifdef _WIN32
#define force_inline		__forceinline
#define abstract_class		class __declspec(novtable)

#else
#define force_inline		inline
#define abstract_class		class
#endif // _WIN32


#define ERRORS

//#define DEBUG_ENGINE
//#define DEBUG_EVENT
//#define DEBUG_AUDIO
//#define DEBUG_ECS
#define DEBUG_RENDER




#ifdef ERRORS
#define LOG_ERROR(msg) \
		do { std::cout << RED << "[ ERROR ] " << RESET << msg << '\n'; } while(0);
#else
#define LOG_ERROR(x)
#endif //LOG_ERROR




#ifdef DEBUG_ENGINE
#define ENGINE_LOG(msg) \
		do { std::cout << CYAN << "[ ENGINE ] " << RESET << msg << '\n'; } while(0);
#else
#define LOG_ENGINE(x)
#endif // DEBUG_ENGINE




#ifdef DEBUG_RENDER
#define RENDER_LOG(msg) \
		do {std::cout << LIGHT_BLUE << "[ RENDER ] " << RESET << msg << '\n'; } while(0);
#else
#define RENDER_LOG(msg)
#endif // DEBUG_RENDER



#ifdef DEBUG_AUDIO
#define LOG_AUDIO(msg) \
				do { std::cout << MAGENTA << "[ AUDIO ] " << RESET << msg << '\n'; } while(0);
#else
#define LOG_AUDIO(x)
#endif // DEBUG AUDIO




#define EV_ASSERT_IS_EVENT(T) \
			static_assert(std::is_base_of_v<BaseEvent, T> and !std::is_same_v<BaseEvent, T>, "Event must be derived from BaseEvent")

#ifdef DEBUG_EVENT
#define EVENT_LOG(x) \
				do { std::cout << "[EVENT] " << x << '\n'; } while(0);
#else
#define EVENT_LOG(x)
#endif // DEBUG_ENGINE


#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"
#define STRIKETHROUGH "\033[9m"
#define REVERSE     "\033[7m"

#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

#define LIGHT_BLACK     "\033[90m"
#define LIGHT_RED       "\033[91m"
#define LIGHT_GREEN     "\033[92m"
#define LIGHT_YELLOW    "\033[93m"
#define LIGHT_BLUE      "\033[94m"
#define LIGHT_MAGENTA   "\033[95m"
#define LIGHT_CYAN      "\033[96m"
#define LIGHT_WHITE     "\033[97m"

#define BG_BLACK        "\033[40m"
#define BG_RED          "\033[41m"
#define BG_GREEN        "\033[42m"
#define BG_YELLOW       "\033[43m"
#define BG_BLUE         "\033[44m"
#define BG_MAGENTA      "\033[45m"
#define BG_CYAN         "\033[46m"
#define BG_WHITE        "\033[47m"

#define BG_LIGHT_BLACK     "\033[100m"
#define BG_LIGHT_RED       "\033[101m"
#define BG_LIGHT_GREEN     "\033[102m"
#define BG_LIGHT_YELLOW    "\033[103m"
#define BG_LIGHT_BLUE      "\033[104m"
#define BG_LIGHT_MAGENTA   "\033[105m"
#define BG_LIGHT_CYAN      "\033[106m"
#define BG_LIGHT_WHITE     "\033[107m"


// GLOBAL ALIAS

using BitFlags = uint64_t;


template<typename T, int NULL_VAL>
struct GenerateID {
	inline static T Get() {
		return globalID++;
	}
private:
	inline static T globalID = NULL_VAL + 1;
};

namespace settings {

	////////////////////////////////////////////////////////////////////////
	// ECS

	// Defines how much memory needs to be reserved for components
	inline constexpr unsigned int MAX_POOL_CAPACITY = 1000000;
	inline constexpr unsigned int MAX_ENTITY_COUNT	= 1000000;

	// Defines how much containers needs to be reserved for components
	inline constexpr unsigned int MAX_COMPONENT_TYPES_COUNT = 10;

	////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////
	// Audio

	// Defines how much memory needs to be reserved for audio
	inline constexpr unsigned int MAX_SOUNDS_COUNT = 1000;
	inline constexpr unsigned int MAX_CHANNELS_COUNT = 700;

	////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////
	// Events

	// Defines how much containers needs to be reserved for events
	inline constexpr unsigned int MAX_EVENT_TYPES = 20;

	////////////////////////////////////////////////////////////////////////


}