#pragma once
#include "core_common.hpp"
#include "core/core_pch.hpp"
namespace lum {
	using SeverityMask = Bit;
	enum class LogSeverity : SeverityMask {
		FATAL = 0b0000'0001,
		ERROR = 0b0000'0010,
		WARN = 0b0000'0100,
		INFO = 0b0000'1000,
		DEBUG = 0b0001'0000
	};
	inline SeverityMask operator|( LogSeverity s1, LogSeverity s2 ) {
		return static_cast<SeverityMask>(s1) | static_cast<SeverityMask>(s2);
	}
	inline SeverityMask operator|( SeverityMask s1, LogSeverity s2 ) {
		return s1 | static_cast<SeverityMask>(s2);
	}
	inline SeverityMask operator|( LogSeverity s1, SeverityMask s2 ) {
		return static_cast<SeverityMask>(s1) | s2;
	}
	inline SeverityMask operator|=( SeverityMask& s1, LogSeverity s2 ) {
		return s1 |= static_cast<SeverityMask>(s2);
	}
	inline SeverityMask operator&( LogSeverity s1, LogSeverity s2 ) {
		return static_cast<SeverityMask>(s1) & static_cast<SeverityMask>(s2);
	}
	inline SeverityMask operator&( SeverityMask s1, LogSeverity s2 ) {
		return s1 & static_cast<SeverityMask>(s2);
	}
	inline SeverityMask operator&(LogSeverity s1, SeverityMask s2 ) {
		return static_cast<SeverityMask>(s1) & s2;
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

		inline void EnableLog	( LogSeverity sev ) {
			g_severity |= sev;
		}
		inline void DisableLog	( LogSeverity sev ) {
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