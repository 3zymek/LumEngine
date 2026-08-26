//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines the engine logging system with severity filtering,
//          log buffering and formatted debug output.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"
#include "Core/Utils/Flags.hpp"
#include "Core/Utils/FormatString.hpp"

namespace lum {

#	define LUM_MAX_LOGS 128

	/* @brief Defines available log severity levels.
	* Controls the importance level of logged messages.
	*/
	enum class LogSeverityLevel : uint8 {
		Fatal, // Critical engine failure.
		Error, // Recoverable error.
		Warn,  // Warning message.
		Info,  // General information.
		Debug  // Debug-only information.
	};


	/* @brief Stores a single log message entry.
	* Contains message text, source location and severity information.
	*/
	struct LogEntry {

		/* @brief Timestamp of the log entry in milliseconds. */
		String m_Time = "";

		/* @brief Formatted log message text. */
		String m_Message = "";

		/* @brief Function where the log was generated. */
		String m_Function = "";

		/* @brief File where the log was generated. */
		String m_File = "";

		/* @brief Line number where the log was generated. */
		uint32 m_Line = 0;

		/* @brief Severity level of this message. */
		LogSeverityLevel m_Severity{};

	};

	namespace detail {

		/* @brief Fixed-size ring buffer storing log entries.
		*
		* Automatically removes the oldest entries when the maximum
		* capacity is exceeded.
		*/
		struct LogBuffer {

			/* @brief Creates a log buffer with a maximum entry count.
			* @param maxLogs Maximum number of stored logs.
			*/
			LogBuffer( uint32 maxLogs ) : m_MaxLogs( maxLogs ) {}

			/* @brief Returns all stored log entries.
			* @return Constant reference to the internal log container.
			*/
			const std::deque<LogEntry>& GetLogs( ) const {
				return m_Logs;
			}

			/* @brief Adds a new log entry.
			* Removes the oldest entry if the buffer is full.
			* @param entry Log entry to store.
			*/
			void Push( const LogEntry& entry ) {

				if (m_Logs.size( ) >= m_MaxLogs)
					m_Logs.pop_front( );

				m_Logs.push_back( entry );
			}

			/* @brief Removes all stored log entries. */
			void Clear( ) {
				m_Logs.clear( );
			}


		private:

			/* @brief Maximum amount of stored logs. */
			uint32 m_MaxLogs = 0;

			/* @brief Stored log entries. */
			std::deque<LogEntry> m_Logs;
		};

	} // namespace lum::detail


	/* @brief Global engine logger. (Singleton)
	*
	* Provides centralized logging functionality with message storage
	* and formatted output support.
	*
	* Accessed through Logger::Get().
	*/
	class LUM_API Logger {
	public:

		/* @brief Returns the global logger instance.
		* @return Reference to the singleton logger.
		*/
		static Logger& Get( ) {
			static Logger log;
			return log;
		}

		/* @brief Returns all stored log entries.
		* @return Constant reference to stored logs.
		*/
		const std::deque<LogEntry>& GetLogs( ) const {
			return m_Logs.GetLogs( );
		}

		/* @brief Clears all stored log entries. */
		void ClearLogs( ) {
			m_Logs.Clear( );
		}

		/* @brief Formats and stores a log message.
		*
		* Formats string arguments into an internal buffer and constructs
		* a LogEntry with calling site context from source_location.
		*
		* @tparam tArgs Variadic formatting arguments.
		*
		* @param sev Severity level of the message.
		* @param loc Automatic source location context (file, function, line).
		* @param msg Message format string.
		* @param args Formatting arguments passed to FormatString.
		*/
		template<typename... tArgs>
		void LogCmd(
			LogSeverityLevel sev,
			std::source_location loc,
			const String& msg,
			tArgs&&... args
		) {

			char formatMsg[ k_MaxLogMessageLength ]{};

			FormatString(
				formatMsg,
				msg.data( ),
				std::forward<tArgs>( args )...
			);

			LogEntry entry{};

			char time[ 16 ]{};
			get_time( time );

			entry.m_Message = String( formatMsg );
			entry.m_Severity = sev;
			entry.m_Time = time;
			entry.m_File = loc.file_name( );
			entry.m_Function = loc.function_name( );
			entry.m_Line = loc.line( );

			entry.m_Function = clean_function_name( entry.m_Function );

			m_Logs.Push( entry );
			m_TempStrings.clear( );

			std::cout << formatMsg << "\n";

		}


	private:

		/* @brief Maximum formatted message length. */
		static inline constexpr uint32 k_MaxLogMessageLength = 1024;
		static inline constexpr uint32 k_MaxTempStrings = 10;

		/* @brief Internal log storage. */
		detail::LogBuffer m_Logs{ LUM_MAX_LOGS };

		/* @brief Temporary strings to secure dangling pointers. */
		std::vector<String> m_TempStrings{ };

		/* @brief Private constructor for singleton pattern. */
		Logger( ) {
			m_TempStrings.reserve( k_MaxTempStrings );
		}

		/* @brief Returns current time formatted as HH:MM:SS.
		* @param out Pointer to the destination buffer (at least 16 bytes).
		*/
		void get_time( char* out ) {
			using namespace std::chrono;

			uint64 time = duration_cast<milliseconds>(system_clock::now( ).time_since_epoch( )).count( );
			std::time_t t = time / 1000;
			std::tm tm{};

			localtime_s( &tm, &t );

			std::strftime( out, 16, "%H:%M:%S", &tm );
		}

		/* @brief Strips return types, calling conventions and argument lists from raw function signatures. */
		constexpr StringView clean_function_name( StringView sig ) {

			usize callConv = sig.find( "__cdecl " );
			if (callConv != StringView::npos) {
				sig.remove_prefix( callConv + 8 );
			}

			usize paren = sig.find( '(' );
			if (paren != StringView::npos) {
				sig = sig.substr( 0, paren );
			}

			usize lastSpace = sig.rfind( ' ' );
			if (lastSpace != StringView::npos) {
				sig.remove_prefix( lastSpace + 1 );
			}

			return sig;
		}

	};

} // namespace lum