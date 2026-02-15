//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Logs messages form all over the engine for debugging and testing
// 
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"
#include "core/core_common.hpp"
#include "core/utils/flags.hpp"

namespace lum {

	using SeverityMask = bitfield;
	enum class LogSeverity : SeverityMask {
		Fatal = 0b0000'0001,
		Error = 0b0000'0010,
		Warn = 0b0000'0100,
		Info = 0b0000'1000,
		Debug = 0b0001'0000,
		All = Fatal | Error | Warn | Info | Debug
	};

	class Logger {
	public:

		inline static Logger& Get( ) {
			static Logger log;
			return log;
		}

		inline constexpr void EnableLog ( Flags<LogSeverity> sev ) {
			mSeverity.Enable(sev);
		}

		inline constexpr void DisableLog (Flags<LogSeverity> sev) {
			mSeverity.Disable(sev);
		}

		template<usize fileL, usize funcL, typename... Args>
		void LogCmd (
			LogSeverity sev,
			const char(&file)[fileL],
			const char(&func)[funcL],
			int32 line,
			ccharptr msg,
			Args&&... args
		) 
		{
			if (!mSeverity.Has(sev)) return;
			if (mLastLogs[0] == HashStr(msg)) return;
			if (mLastLogs[1] == HashStr(msg)) return;
			if (mLastLogs[2] == HashStr(msg)) return;
			if (mLastLogs[3] == HashStr(msg)) return;
			if (mLastLogs[4] == HashStr(msg)) return;

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
			std::printf(msg, std::forward<Args>(args)...);
			std::printf("%c", '\n');

			mLastLogs[0] = HashStr(msg);

		}

	private:

		LUM_COMPILE_VARIABLE
		static uint32 MAX_COLOR_LENGTH = 6; // With \0

		LUM_COMPILE_VARIABLE
		static uint32 MAX_SEVERITY_LENGTH = 6; // With \0

		LUM_COMPILE_VARIABLE
		static uint32 DESCRIPTION_LENGTH = 128;

		char mDescBuffer			[DESCRIPTION_LENGTH]	{};
		char mDescTempBuffer		[DESCRIPTION_LENGTH]	{};
		char mSeverityTempBuffer	[MAX_SEVERITY_LENGTH]	{};
		char mCentertedSeverity		[16]					{};
		char mSevColorBuffer		[MAX_COLOR_LENGTH]		{};

		uint64 mLastLogs[5]{};
		Flags<LogSeverity> mSeverity = LogSeverity::Fatal | LogSeverity::Debug | LogSeverity::Warn | LogSeverity::Error;
		
		template<usize L>
		void center_custom(charptr out, usize out_size, const char(&s)[L], usize left_width, usize right_width) {

			if (L <= 1) return;

			usize len = strlen(s);

			usize rightPad = right_width - len;

			usize copyLen = std::min<usize>(len, out_size - left_width);
			usize padRight = std::min<usize>(rightPad, out_size - left_width - copyLen);

			std::memset(out, ' ', left_width);
			std::memcpy(out + left_width, s, copyLen);
			std::memset(out + left_width + copyLen, ' ', padRight);

		}

		template<usize fileL>
		ccharptr extract_filename(const char(&path)[fileL]) {
			ccharptr lastSlash = nullptr;

			for (size_t i = 0; i < fileL - 1; ++i) {
				if (path[i] == '/' || path[i] == '\\') {
					lastSlash = &path[i];
				}
			}

			return lastSlash ? lastSlash + 1 : path;
		}

		void output_time();

		void to_string	( charptr out, LogSeverity sev );
		void to_color	( charptr out, LogSeverity sev );

		Logger() = default;

	};
}