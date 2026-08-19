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
		String mTime = "";

		/* @brief Formatted log message text. */
		String mMessage = "";

		/* @brief Function where the log was generated. */
		String mFunction = "";

		/* @brief File where the log was generated. */
		String mFile = "";

		/* @brief Line number where the log was generated. */
		uint32 mLine = 0;

		/* @brief Severity level of this message. */
		LogSeverityLevel mSeverity{};

	};


	/* @brief Fixed-size ring buffer storing log entries.
	*
	* Automatically removes the oldest entries when the maximum
	* capacity is exceeded.
	*/
	struct LogBuffer {

		/* @brief Creates a log buffer with a maximum entry count.
		* @param maxLogs Maximum number of stored logs.
		*/
		LogBuffer( uint32 maxLogs ) : mMaxLogs( maxLogs ) {}

		/* @brief Returns all stored log entries.
		* @return Constant reference to the internal log container.
		*/
		const std::deque<LogEntry>& GetLogs( ) const {
			return mLogs;
		}

		/* @brief Adds a new log entry.
		* Removes the oldest entry if the buffer is full.
		* @param entry Log entry to store.
		*/
		void Push( const LogEntry& entry ) {

			if (mLogs.size( ) >= mMaxLogs)
				mLogs.pop_front( );

			mLogs.push_back( entry );
		}

		/* @brief Removes all stored log entries. */
		void Clear( ) {
			mLogs.clear( );
		}


	private:

		/* @brief Maximum amount of stored logs. */
		uint32 mMaxLogs = 0;

		/* @brief Stored log entries. */
		std::deque<LogEntry> mLogs;
	};


	/* @brief Global engine logger.
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
			return mLogs.GetLogs( );
		}

		/* @brief Clears all stored log entries. */
		void ClearLogs( ) {
			mLogs.Clear( );
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

			char formatMsg[ skMaxLogMessageLength ]{};

			FormatString(
				formatMsg,
				msg.data( ),
				std::forward<tArgs>( args )...
			);

			LogEntry entry{};

			char time[ 16 ]{};
			get_time( time );

			entry.mMessage = String( formatMsg );
			entry.mSeverity = sev;
			entry.mTime = time;
			entry.mFile = loc.file_name( );
			entry.mFunction = loc.function_name( );
			entry.mLine = loc.line( );

			entry.mFunction = clean_function_name( entry.mFunction );

			mLogs.Push( entry );
			mTempStrings.clear( );

		}


	private:

		/* @brief Maximum formatted message length. */
		inline static constexpr uint32 skMaxLogMessageLength = 1024;
		inline static constexpr uint32 mMaxTempStrings = 10;

		/* @brief Internal log storage. */
		LogBuffer mLogs{ LUM_MAX_LOGS };

		/* @brief Temporary strings to secure dangling pointers. */
		std::vector<String> mTempStrings{ };

		/* @brief Private constructor for singleton pattern. */
		Logger( ) {
			mTempStrings.reserve( mMaxTempStrings );
		}

		/* @brief Returns current time formatted as HH:MM:SS.
		* @param out Pointer to the destination buffer (at least 16 bytes).
		*/
		static void get_time( char* out ) {
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