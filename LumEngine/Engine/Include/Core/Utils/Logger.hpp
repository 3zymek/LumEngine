//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Logs messages from all over the engine for debugging and testing.
//
//=============================================================================//
#pragma once
#include "Core/Types.hpp"
#include "Core/Utils/Flags.hpp"

namespace lum {

#	define LUM_MAX_LOGS 128

	// Combinable bitmask severity levels.
	enum class LogSeverityLevel : uint8 {
		Fatal,
		Error,
		Warn,
		Info,
		Debug
	};

	// Data for a single log entry.
	struct LogEntry {
		uint64      mTime = 0;
		String      mMessage = "";
		ccharptr    mFunction = "";
		ccharptr    mFile = "";
		uint32      mLine = 0;
		LogSeverityLevel mSeverity{};
	};

	// Fixed-size ring buffer for log entries.
	struct LogRingBuffer {

		LogRingBuffer( uint32 maxLogs ) : mMaxLogs( maxLogs ) { }

		const std::deque<LogEntry>& GetLogs( ) const { return mLogs; }

		// Pushes entry, drops oldest if full.
		void Push( const LogEntry& entry ) {
			if (mLogs.size( ) >= mMaxLogs)
				mLogs.pop_front( );
			mLogs.push_back( entry );
		}
		void Clear( ) { mLogs.clear( ); }

	private:
		uint32 mMaxLogs = 0;
		std::deque<LogEntry> mLogs;
	};

	// Singleton logger with severity filtering and formatted output.
	// Access via Logger::Get().
	class LUM_API Logger {
	public:

		static Logger& Get( ) {
			static Logger log;
			return log;
		}

		const std::deque<LogEntry>& GetLogs( ) const { return mLogs.GetLogs( ); }
		void ClearLogs( ) { mLogs.Clear( ); }

		// Formats unix timestamp into human-readable string.
		static void FormatTime( uint64 timestamp, charptr out );

		// Formats and stores a log entry. Skips if severity is disabled.
		template<usize tFileL, usize tFuncL, typename... tArgs>
		void LogCmd(
			LogSeverityLevel sev,
			const char( &file )[ tFileL ],
			const char( &func )[ tFuncL ],
			int32 line,
			StringView msg,
			tArgs&&... args
		) {

			char formatMsg[ skMaxLogMessageLength ]{};
			std::snprintf( formatMsg, sizeof( formatMsg ), msg.data( ), std::forward<tArgs>( args )... );

			LogEntry entry;
			entry.mMessage = String(formatMsg);
			entry.mSeverity = sev;
			entry.mTime = get_time( );
			mLogs.Push( entry );
		}

	private:

		inline static constexpr uint32 skMaxLogMessageLength = 128;	// Max formatted message length.
		LogRingBuffer mLogs { LUM_MAX_LOGS };						// Stored log entries.

		Logger( ) = default;

		// Returns current unix timestamp in milliseconds.
		static uint64 get_time( );

		// Extracts filename from full file path.
		template<usize tLength>
		ccharptr extract_filename( const char( &path )[ tLength ] ) {
			ccharptr lastSlash = nullptr;
			for (usize i = 0; i < tLength - 1; ++i)
				if (path[ i ] == '/' || path[ i ] == '\\')
					lastSlash = &path[ i ];
			return lastSlash ? lastSlash + 1 : path;
		}
	};

} // namespace lum