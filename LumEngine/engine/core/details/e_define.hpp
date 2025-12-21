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
#define DEBUG_AUDIO
//#define DEBUG_ECS

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


// Reset / style
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"
#define STRIKETHROUGH "\033[9m"
#define REVERSE     "\033[7m"

// Kolory tekstu
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

// Kolory t³a
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