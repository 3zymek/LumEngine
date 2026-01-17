#pragma once
#include "core/core_pch.hpp"
#include "core/core_common.hpp"
namespace lum {

	using SeverityMask = Bit;
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

	enum class InitStatus {
		OK,
		FAIL
	};

	class Logger {
	public:
		inline static Logger& Get( ) {
			static Logger log;
			return log;
		}

		inline constexpr void EnableLog	( SeverityMask sev ) {
			g_severity |= sev;
		}
		inline constexpr void EnableLog( LogSeverity sev ) {
			g_severity |= sev;
		}
		inline constexpr void DisableLog( SeverityMask sev ) {
			g_severity &= ~sev;
		}
		inline constexpr void DisableLog	( LogSeverity sev ) {
			g_severity &= ~static_cast<SeverityMask>(sev);
		}
		inline void LogInit		( InitStatus stat, const char* msg ) {
			std::cout << "[" << ToColor(stat) << ToString(stat) << cmdcolor::reset << "] " << msg << '\n';
		}

		void Log( LogSeverity, const char*, const char*, int, const std::string& );

	private:

		SeverityMask g_severity = LogSeverity::FATAL | LogSeverity::DEBUG | LogSeverity::WARN | LogSeverity::ERROR;

		std::string Center		( const std::string& s, size_t width );
		std::string CenterCustom( const std::string& s, size_t left_width, size_t right_width );

		void OutputTime();

		std::string ToString	( LogSeverity sev );
		std::string ToColor		( LogSeverity sev );
		std::string ToString	( InitStatus stat );
		std::string ToColor		( InitStatus stat );

		Logger() = default;

	};
}