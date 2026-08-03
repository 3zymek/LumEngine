//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines the engine logging system with severity filtering,
//          log buffering and formatted debug output.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"
#include "Core/Utils/Flags.hpp"

namespace lum {

#	define LUM_MAX_LOGS 128

	/* @brief Defines available log severity levels.
	*
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
	*
	* Contains message text, source location and severity information.
	*/
	struct LogEntry {

		/* @brief Timestamp of the log entry in milliseconds. */
		uint64 mTime = 0;

		/* @brief Formatted log message text. */
		String mMessage = "";

		/* @brief Function where the log was generated. */
		ccharptr mFunction = "";

		/* @brief File where the log was generated. */
		ccharptr mFile = "";

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
		LogBuffer( uint32 maxLogs ) : mMaxLogs( maxLogs ) { }

		/* @brief Returns all stored log entries.
		* @return Constant reference to the internal log container.
		*/
		const std::deque<LogEntry>& GetLogs( ) const {
			return mLogs;
		}

		/* @brief Adds a new log entry.
		*
		* Removes the oldest entry if the buffer is full.
		*
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

		/* @brief Converts timestamp into readable time format.
		*
		* @param timestamp Unix timestamp in milliseconds.
		* @param out Output character buffer.
		*/
		static void FormatTime( uint64 timestamp, charptr out );

		/* @brief Formats and stores a log message.
		*
		* Uses printf-style formatting and stores the generated entry
		* inside the internal log buffer.
		*
		* @tparam tFileL Length of the source file string.
		* @tparam tFuncL Length of the source function string.
		* @tparam tArgs Additional formatting arguments.
		*
		* @param sev Severity level of the message.
		* @param file Source file name.
		* @param func Function name.
		* @param line Source code line.
		* @param msg Message format string.
		* @param args Formatting arguments.
		*/
		template<usize tFileL, usize tFuncL, typename... tArgs>
		void LogCmd(
			LogSeverityLevel sev,
			const char( &file )[ tFileL ],
			const char( &func )[ tFuncL ],
			int32 line,
			const String& msg,
			tArgs&&... args
		) {

			char formatMsg[ skMaxLogMessageLength ]{};

			std::snprintf(
				formatMsg,
				sizeof( formatMsg ),
				msg.data( ),
				format_args( std::forward<tArgs>( args ) )...
			);

			LogEntry entry;

			entry.mMessage = String( formatMsg );
			entry.mSeverity = sev;
			entry.mTime = get_time( );

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

		/* @brief Returns current Unix timestamp in milliseconds.
		* @return Current timestamp.
		*/
		static uint64 get_time( );

		/* @brief Extracts filename from full path.
		*
		* @param path File path string.
		* @return Pointer to filename portion.
		*/
		template<usize tLength>
		ccharptr extract_filename( const char( &path )[ tLength ] ) {

			ccharptr lastSlash = nullptr;

			for (usize i = 0; i < tLength - 1; ++i)
				if (path[ i ] == '/' || path[ i ] == '\\')
					lastSlash = &path[ i ];

			return lastSlash ? lastSlash + 1 : path;
		}

		ccharptr format_args( const String& str ) {
			return str.c_str( );
		}
		ccharptr format_args( StringView str ) {
			return str.data( );
		}
		ccharptr format_args( String&& val ) {

			if (mTempStrings.size( ) <= mMaxTempStrings)
				mTempStrings.push_back( std::move( val ) );
			return mTempStrings.back( ).c_str( );

		}

		template<typename tType>
		tType&& format_args( tType&& val ) {
			return std::forward<tType>( val );
		}

	};

} // namespace lum