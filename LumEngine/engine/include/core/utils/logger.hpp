//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Logs messages form all over the engine for debugging and testing.
// 
//=============================================================================//
#pragma once

#include "core/core_common.hpp"
#include "core/utils/flags.hpp"

namespace lum {

	using SeverityMask = bitfield;
#	define LUM_MAX_LOGS 32

	/* @brief Bitmask enum defining log severity levels.
	* Can be combined to enable or disable multiple levels at once.
	*/
	enum class LogSeverity : SeverityMask {
		Fatal = 0b0000'0001,
		Error = 0b0000'0010,
		Warn = 0b0000'0100,
		Info = 0b0000'1000,
		Debug = 0b0001'0000,
		All = Fatal | Error | Warn | Info | Debug
	};
	LUM_ENUM_OPERATIONS( lum::LogSeverity );

	struct FLogEntry {
		uint64 mTime = 0;
		String mMessage = "";
		ccharptr mFunction = "";
		ccharptr mFile = "";
		uint32 mLine = 0;
		LogSeverity mSeverity{};
	};

	struct LogBuffer {

		LogBuffer( uint32 maxLogs ) : mMaxLogs( maxLogs ) { }

		const std::deque<FLogEntry>& GetLogs( ) const { return mLogs; }
		void Push( FLogEntry& entry ) {
			if (mLogs.size( ) >= mMaxLogs)
				mLogs.pop_front( );
			mLogs.push_back( entry );
		}

	private:

		uint32 mMaxLogs = 0;
		std::deque<FLogEntry> mLogs;

	};



	/* @brief Singleton logger with severity filtering and colored console output.
	*
	* Outputs formatted log messages to stdout with timestamp, severity,
	* source file, line number and function name.
	* Duplicate consecutive messages are suppressed via hash comparison.
	*
	* @note Access via Logger::Get(). Non-constructible externally.
	*/
	class Logger {
	public:

		/* @brief Returns the singleton Logger instance. */
		inline static Logger& Get( ) {
			static Logger log;
			return log;
		}

		inline const std::deque<FLogEntry>& GetLogs( ) const { return mLogs.GetLogs( ); }

		/* @brief Enables logging for the given severity flags.
		* @param sev Severity levels to enable.
		*/
		inline constexpr void EnableLog( Flags<LogSeverity> sev ) {
			mSeverity.Enable( sev );
		}

		/* @brief Disables logging for the given severity flags.
		* @param sev Severity levels to disable.
		*/
		inline constexpr void DisableLog( Flags<LogSeverity> sev ) {
			mSeverity.Disable( sev );
		}

		/* @brief Converts a LogSeverity value to its string representation.
		* @param out Output buffer.
		* @param sev Severity to convert.
		*/
		static StringView SeverityToString( LogSeverity sev );

		static void FormatTime( uint64 timestamp, charptr out );

		/* @brief Formats and outputs a log message to stdout.
		* Skips output if the severity is disabled or the message was already logged.
		* @param sev    Severity level of the message.
		* @param file   Source file path (resolved to filename only).
		* @param func   Calling function name.
		* @param line   Source line number.
		* @param msg    printf-style format string.
		* @param args   Format arguments.
		*/
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
			std::snprintf(
				formatMsg,
				sizeof( formatMsg ),
				msg.data( ),
				std::forward<tArgs>( args )...
			);

			FLogEntry entry;
			entry.mMessage = formatMsg;
			entry.mSeverity = sev;
			entry.mTime = get_time( );
			mLogs.Push( entry );

		}

	private:

		LogBuffer mLogs{ LUM_MAX_LOGS };

		inline constexpr static uint32 sMaxDescriptionLength = 128; /* Max formatted description buffer length. */

		/* @brief Active severity filter mask. Defaults to all levels enabled. */
		Flags<LogSeverity> mSeverity{ LogSeverity::All };

		/* @brief Extracts the filename from a full file path at compile time.
		* @param path Full source file path.
		* @return Pointer to the filename portion of the path.
		*/
		template<usize tL>
		ccharptr extract_filename( const char( &path )[ tL ] ) {
			ccharptr lastSlash = nullptr;
			for (usize i = 0; i < tL - 1; ++i) {
				if (path[ i ] == '/' || path[ i ] == '\\') {
					lastSlash = &path[ i ];
				}
			}
			return lastSlash ? lastSlash + 1 : path;
		}

		static uint64 get_time( );

		Logger( ) = default;
	};

} // namespace lum