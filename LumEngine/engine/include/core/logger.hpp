#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
namespace lum {

	using SeverityMask = bitfield;
	enum class LogSeverity : SeverityMask {
		FATAL = 0b0000'0001,
		ERROR = 0b0000'0010,
		WARN = 0b0000'0100,
		INFO = 0b0000'1000,
		DEBUG = 0b0001'0000,
		ALL = FATAL | ERROR | WARN | INFO | DEBUG
	};
	inline constexpr SeverityMask operator|(LogSeverity a, LogSeverity b) {
		return static_cast<SeverityMask>(a) | static_cast<SeverityMask>(b);
	}
	inline constexpr SeverityMask operator|(SeverityMask a, LogSeverity b) {
		return a | static_cast<SeverityMask>(b);
	}
	inline constexpr SeverityMask operator|(LogSeverity a, SeverityMask b) {
		return static_cast<SeverityMask>(a) | b;
	}
	inline constexpr SeverityMask operator&(SeverityMask mask, LogSeverity sev) {
		return mask & static_cast<SeverityMask>(sev);
	}
	inline constexpr SeverityMask& operator|=(SeverityMask& mask, LogSeverity sev) {
		mask |= static_cast<SeverityMask>(sev);
		return mask;
	}

	class Logger {
	public:
		inline static Logger& Get( ) {
			static Logger log;
			return log;
		}

		inline constexpr void enable_log	( SeverityMask sev ) {
			gSeverity |= sev;
		}
		inline constexpr void enable_log( LogSeverity sev ) {
			gSeverity |= sev;
		}
		inline constexpr void disable_log( SeverityMask sev ) {
			gSeverity &= ~sev;
		}
		inline constexpr void disable_log	( LogSeverity sev ) {
			gSeverity &= ~static_cast<SeverityMask>(sev);
		}

		template<usize fileL>
		const char* filename_from_path(const char(&file)[fileL]) {
			const char* lastSlash = nullptr;

			for (size_t i = 0; i < fileL - 1; ++i) {
				if (file[i] == '/' || file[i] == '\\') {
					lastSlash = &file[i];
				}
			}

			return lastSlash ? lastSlash + 1 : file;
		}

		template<usize fileL, usize funcL, typename... Args>
		void log_cmd(
			LogSeverity sev, 
			const char(&file)[fileL], 
			const char(&func)[funcL], 
			int32 line, 
			ccharptr msg, 
			Args&&... args
		) 
		{
			if (!(gSeverity & sev)) return;
			if (lastLog == hash(msg)) return;

			// Cleanup
			std::memset(descFinalBuffer, 0, sizeof(descFinalBuffer));
			std::memset(descBuffer, 0, sizeof(descBuffer));
			std::memset(centertedSeverity, 0, sizeof(centertedSeverity));
			std::memset(sevColorBuffer, 0, sizeof(sevColorBuffer));
			std::memset(severityBuffer, 0, sizeof(severityBuffer));

			_OutputTime();
			
			_ToString(severityBuffer, sev);
			_CenterCustom(centertedSeverity, 8, severityBuffer, 1, 6);
			_ToColor(sevColorBuffer, sev);

			const char* filename = filename_from_path(file);

			int32 descLen = 
				std::snprintf(
					descBuffer, 
					sizeof(descBuffer), 
					"%s:%s%d%s %s", 
					filename, 
					cmdcolor::cyan, 
					line, 
					cmdcolor::reset, 
					func
				);

			_CenterCustom(descFinalBuffer, sizeof(descFinalBuffer), descBuffer, 2, 52);

			std::printf("[%s%s%s][%s] ", sevColorBuffer, centertedSeverity, cmdcolor::reset, descFinalBuffer);
			std::printf(msg, std::forward<Args>(args)...);
			std::printf("%c", '\n');


			lastLog = hash(msg);

		}

	private:

		LUM_COMPILE_VARIABLE
		static uint32 skMaxColorLength = 6; // With \0

		LUM_COMPILE_VARIABLE
		static uint32 skMaxSeverityLength = 6; // With \0

		LUM_COMPILE_VARIABLE
		static uint32 skDescriptionLength = 128;

		char descFinalBuffer	[skDescriptionLength]	{};
		char descBuffer			[skDescriptionLength]	{};
		char severityBuffer		[skMaxSeverityLength]	{};
		char centertedSeverity	[16]					{};
		char sevColorBuffer		[skMaxColorLength]		{};

		uint64 lastLog = 0;
		SeverityMask gSeverity = LogSeverity::FATAL | LogSeverity::DEBUG | LogSeverity::WARN | LogSeverity::ERROR;
		
		template<usize L>
		void _CenterCustom(charptr out, usize out_size, const char(&s)[L], usize left_width, usize right_width) {

			if (L <= 1) return;

			usize len = strlen(s);

			usize right_pad = right_width - len;

			usize copy_len = std::min<usize>(len, out_size - left_width);
			usize pad_right = std::min<usize>(right_pad, out_size - left_width - copy_len);

			std::memset(out, ' ', left_width);
			std::memcpy(out + left_width, s, copy_len);
			std::memset(out + left_width + copy_len, ' ', pad_right);

		}

		void _OutputTime();

		void _ToString	( charptr out, LogSeverity sev );
		void _ToColor	( charptr out, LogSeverity sev );

		Logger() = default;

	};
}