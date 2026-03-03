//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Logs messages form all over the engine for debugging and testing.
// 
//=============================================================================//
#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
#include "core/utils/flags.hpp"
namespace lum {

	using SeverityMask = bitfield;

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
	LUM_ENUM_OPERATIONS(lum::LogSeverity);



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

		/* @brief Enables logging for the given severity flags.
		* @param sev Severity levels to enable.
		*/
		inline constexpr void EnableLog( Flags<LogSeverity> sev ) {
			mSeverity.Enable(sev);
		}

		/* @brief Disables logging for the given severity flags.
		* @param sev Severity levels to disable.
		*/
		inline constexpr void DisableLog( Flags<LogSeverity> sev ) {
			mSeverity.Disable(sev);
		}

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
			const char(&file)[tFileL],
			const char(&func)[tFuncL],
			int32 line,
			ccharptr msg,
			tArgs&&... args
		)
		{

			if (!mSeverity.Has(sev)) return;
			if (mLastLog == HashStr(msg)) return;
			
			// Cleanup
			std::memset(mDescBuffer, 0, sizeof(mDescBuffer));
			std::memset(mDescTempBuffer, 0, sizeof(mDescTempBuffer));
			std::memset(mCentertedSeverity, 0, sizeof(mCentertedSeverity));
			std::memset(mSevColorBuffer, 0, sizeof(mSevColorBuffer));
			std::memset(mSeverityTempBuffer, 0, sizeof(mSeverityTempBuffer));
			
			output_time();
			to_string(mSeverityTempBuffer, sev);
			center_custom(mCentertedSeverity, 8, mSeverityTempBuffer, 1, 6);
			to_color(mSevColorBuffer, sev);
			
			const char* filename = extract_filename(file);

			int32 descLen =
				std::snprintf(
					mDescTempBuffer,
					sizeof(mDescTempBuffer),
					"%s:%s%d%s %s",
					filename,
					cmdcolor::Cyan,
					line,
					cmdcolor::Reset,
					func
				);
			
			center_custom(mDescBuffer, sizeof(mDescBuffer), mDescTempBuffer, 2, 64);
			
			std::printf("[%s%s%s][%s] ", mSevColorBuffer, mCentertedSeverity, cmdcolor::Reset, mDescBuffer);
			std::printf(msg, std::forward<tArgs>(args)...);
			std::printf("%c", '\n');

			mLastLog = HashStr(msg);

		}

	private:

		inline constexpr
		static uint32 sMaxColorLength = 6;   /* Max ANSI color code length (with \0). */

		inline constexpr
		static uint32 sMaxSeverityLength = 6;   /* Max severity string length (with \0). */

		inline constexpr
		static uint32 sMaxDescriptionLength = 128; /* Max formatted description buffer length. */

		/* @brief Buffers used for formatting log output. */
		char mDescBuffer[sMaxDescriptionLength]{};
		char mDescTempBuffer[sMaxDescriptionLength]{};
		char mSeverityTempBuffer[sMaxSeverityLength]{};
		char mCentertedSeverity[16]{};
		char mSevColorBuffer[sMaxColorLength]{};

		/* @brief Hashes of recently logged messages used for duplicate suppression. */
		uint64 mLastLog{};

		/* @brief Active severity filter mask. Defaults to all levels enabled. */
		Flags<LogSeverity> mSeverity{ LogSeverity::All };

		/* @brief Centers string s within a field, padding with spaces.
		* @param out       Output buffer.
		* @param outSize   Size of the output buffer.
		* @param s         String to center.
		* @param leftWidth Left padding width.
		* @param rightWidth Total field width.
		*/
		template<usize tL>
		void center_custom( charptr out, usize outSize, const char(&s)[tL], usize leftWidth, usize rightWidth ) {
			if (tL <= 1) return;
			usize len = strlen(s);
			usize rightPad = rightWidth - len;
			usize copyLen = std::min<usize>(len, outSize - leftWidth);
			usize padRight = std::min<usize>(rightPad, outSize - leftWidth - copyLen);
			std::memset(out, ' ', leftWidth);
			std::memcpy(out + leftWidth, s, copyLen);
			std::memset(out + leftWidth + copyLen, ' ', padRight);
		}

		/* @brief Extracts the filename from a full file path at compile time.
		* @param path Full source file path.
		* @return Pointer to the filename portion of the path.
		*/
		template<usize tL>
		ccharptr extract_filename( const char(&path)[tL] ) {
			ccharptr lastSlash = nullptr;
			for (size_t i = 0; i < tL - 1; ++i) {
				if (path[i] == '/' || path[i] == '\\') {
					lastSlash = &path[i];
				}
			}
			return lastSlash ? lastSlash + 1 : path;
		}

		/* @brief Writes the current time to stdout.*/
		void output_time( );

		/* @brief Converts a LogSeverity value to its string representation.
		* @param out Output buffer.
		* @param sev Severity to convert.
		*/
		void to_string( charptr out, LogSeverity sev );

		/* @brief Writes the ANSI color code for the given severity into out.
		* @param out Output buffer.
		* @param sev Severity to convert.
		*/
		void to_color( charptr out, LogSeverity sev );

		Logger( ) = default;
	};

} // namespace lum