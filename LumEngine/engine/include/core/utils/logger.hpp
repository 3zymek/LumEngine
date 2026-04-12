//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Logs messages from all over the engine for debugging and testing.
//
//=============================================================================//
#pragma once
#include "core/types.hpp"
#include "core/utils/flags.hpp"

namespace lum {

	using SeverityMask = bitfield;

#	define LUM_MAX_LOGS 32

	// Combinable bitmask severity levels.
	enum class LogSeverity : SeverityMask {
		Fatal = 0b0000'0001,
		Error = 0b0000'0010,
		Warn = 0b0000'0100,
		Info = 0b0000'1000,
		Debug = 0b0001'0000,
		All = Fatal | Error | Warn | Info | Debug
	};
	LUM_ENUM_OPERATIONS( lum::LogSeverity );

	// Data for a single log entry.
	struct FLogEntry {
		uint64      mTime = 0;
		String      mMessage = "";
		ccharptr    mFunction = "";
		ccharptr    mFile = "";
		uint32      mLine = 0;
		LogSeverity mSeverity{};
	};

	// Fixed-size ring buffer for log entries.
	struct LogBuffer {

		LogBuffer( uint32 maxLogs ) : mMaxLogs( maxLogs ) { }

		const std::deque<FLogEntry>& GetLogs( ) const { return mLogs; }

		// Pushes entry, drops oldest if full.
		void Push( FLogEntry& entry ) {
			if (mLogs.size( ) >= mMaxLogs)
				mLogs.pop_front( );
			mLogs.push_back( entry );
		}

	private:
		uint32 mMaxLogs = 0;
		std::deque<FLogEntry> mLogs;
	};

	// Singleton logger with severity filtering and formatted output.
	// Access via Logger::Get().
	class Logger {
	public:

		static Logger& Get( ) {
			static Logger log;
			return log;
		}

		const std::deque<FLogEntry>& GetLogs( ) const { return mLogs.GetLogs( ); }

		// Enable/disable specific severity levels.
		void EnableLog( Flags<LogSeverity> sev ) { mSeverity.Enable( sev ); }
		void DisableLog( Flags<LogSeverity> sev ) { mSeverity.Disable( sev ); }

		// Converts severity to string label.
		static StringView SeverityToString( LogSeverity sev );

		// Formats unix timestamp into human-readable string.
		static void FormatTime( uint64 timestamp, charptr out );

		// Formats and stores a log entry. Skips if severity is disabled.
		template<usize tFileL, usize tFuncL, typename... tArgs>
		void LogCmd(
			LogSeverity sev,
			const char( &file )[ tFileL ],
			const char( &func )[ tFuncL ],
			int32 line,
			StringView msg,
			tArgs&&... args
		) {
			if (!mSeverity.Has( sev )) return;

			char formatMsg[ sMaxDescriptionLength ]{};
			std::snprintf( formatMsg, sizeof( formatMsg ), msg.data( ), std::forward<tArgs>( args )... );

			FLogEntry entry;
			entry.mMessage = formatMsg;
			entry.mSeverity = sev;
			entry.mTime = get_time( );
			mLogs.Push( entry );
		}

	private:

		Logger( ) = default;

		inline constexpr static uint32 sMaxDescriptionLength = 128;	// Max formatted message length.
		Flags<LogSeverity> mSeverity { LogSeverity::All };          // Active severity filter.
		LogBuffer mLogs { LUM_MAX_LOGS };                           // Stored log entries.

		// Returns current unix timestamp in milliseconds.
		static uint64 get_time( );

		// Extracts filename from full file path.
		template<usize tL>
		ccharptr extract_filename( const char( &path )[ tL ] ) {
			ccharptr lastSlash = nullptr;
			for (usize i = 0; i < tL - 1; ++i)
				if (path[ i ] == '/' || path[ i ] == '\\')
					lastSlash = &path[ i ];
			return lastSlash ? lastSlash + 1 : path;
		}
	};

} // namespace lum