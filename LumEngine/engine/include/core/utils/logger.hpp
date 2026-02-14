#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
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
			if (lastLog == HashStr(msg)) return;

			// Cleanup
			std::memset(descBuffer, 0, sizeof(descBuffer));
			std::memset(descTempBuffer, 0, sizeof(descTempBuffer));
			std::memset(centertedSeverity, 0, sizeof(centertedSeverity));
			std::memset(sevColorBuffer, 0, sizeof(sevColorBuffer));
			std::memset(severityTempBuffer, 0, sizeof(severityTempBuffer));

			output_time();
			
			to_string(severityTempBuffer, sev);
			center_custom(centertedSeverity, 8, severityTempBuffer, 1, 6);
			to_color(sevColorBuffer, sev);

			const char* filename = extract_filename(file);

			int32 descLen = 
				std::snprintf(
					descTempBuffer, 
					sizeof(descTempBuffer), 
					"%s:%s%d%s %s", 
					filename, 
					cmdcolor::Cyan, 
					line, 
					cmdcolor::Reset, 
					func
				);

			center_custom(descBuffer, sizeof(descBuffer), descTempBuffer, 2, 64);
			
			std::printf("[%s%s%s][%s] ", sevColorBuffer, centertedSeverity, cmdcolor::Reset, descBuffer);
			std::printf(msg, std::forward<Args>(args)...);
			std::printf("%c", '\n');

			lastLog = HashStr(msg);

		}

	private:

		LUM_COMPILE_VARIABLE
		static uint32 skMaxColorLength = 6; // With \0

		LUM_COMPILE_VARIABLE
		static uint32 skMaxSeverityLength = 6; // With \0

		LUM_COMPILE_VARIABLE
		static uint32 skDescriptionLength = 128;

		char descBuffer			[skDescriptionLength]	{};
		char descTempBuffer		[skDescriptionLength]	{};
		char severityTempBuffer	[skMaxSeverityLength]	{};
		char centertedSeverity	[16]					{};
		char sevColorBuffer		[skMaxColorLength]		{};

		uint64 lastLog = 0;
		SeverityMask gSeverity = LogSeverity::Fatal | LogSeverity::Debug | LogSeverity::Warn | LogSeverity::Error;
		
		template<usize L>
		void center_custom(charptr out, usize out_size, const char(&s)[L], usize left_width, usize right_width) {

			if (L <= 1) return;

			usize len = strlen(s);

			usize right_pad = right_width - len;

			usize copy_len = std::min<usize>(len, out_size - left_width);
			usize pad_right = std::min<usize>(right_pad, out_size - left_width - copy_len);

			std::memset(out, ' ', left_width);
			std::memcpy(out + left_width, s, copy_len);
			std::memset(out + left_width + copy_len, ' ', pad_right);

		}

		template<usize fileL>
		const char* extract_filename(const char(&path)[fileL]) {
			const char* lastSlash = nullptr;

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