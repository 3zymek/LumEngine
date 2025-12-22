#pragma once
#include "core_common.hpp"
#include <chrono>
#include <filesystem>
using SeverityMask = Bit;
enum class LogSeverity : SeverityMask {
	FATAL = 0b0000'0001,
	ERROR = 0b0000'0010,
	WARN = 0b0000'0100,
	INFO = 0b0000'1000,
	DEBUG = 0b0001'0000
};
inline SeverityMask operator|(LogSeverity s1, LogSeverity s2) {
	return static_cast<SeverityMask>(s1) | static_cast<SeverityMask>(s2);
}
inline SeverityMask operator|(SeverityMask s1, LogSeverity s2) {
	return s1 | static_cast<SeverityMask>(s2);
}
inline SeverityMask operator|(LogSeverity s1, SeverityMask s2) {
	return static_cast<SeverityMask>(s1) | s2;
}
inline SeverityMask operator|=(SeverityMask& s1, LogSeverity s2) {
	return s1 |= static_cast<SeverityMask>(s2);
}
inline SeverityMask operator&(LogSeverity s1, LogSeverity s2) {
	return static_cast<SeverityMask>(s1) & static_cast<SeverityMask>(s2);
}
inline SeverityMask operator&(SeverityMask s1, LogSeverity s2) {
	return s1 & static_cast<SeverityMask>(s2);
}
inline SeverityMask operator&(LogSeverity s1, SeverityMask s2) {
	return static_cast<SeverityMask>(s1) & s2;
}

enum class InitStatus {
	OK,
	FAIL
};


class Logger {
public:
	inline static Logger& Get() {
		static Logger log;
		return log;
	}

	inline void EnableLog(LogSeverity sev) {
		g_severity |= sev;
	}
	inline void DisableLog(LogSeverity sev) {
		g_severity &= ~static_cast<SeverityMask>(sev);
	}

	inline void Log(LogSeverity sev, const char* file, const char* func, int line, const std::string& msg) {
		if (!(g_severity & sev)) return;

		OutputTime();

		std::filesystem::path p = file;
		std::string filename = p.filename().string();
		std::string severity = ToString(sev);
		std::string info = filename + ":" + cmdcolor::cyan + std::to_string(line) + cmdcolor::reset + " " + func;

		std::cout
			<< "["
			<< ToColor(sev)
			<< CenterCustom(severity, 1, 6)
			<< cmdcolor::reset
			<< "]["
			<< CenterCustom(info, 2, 52)
			<< "] "
			<< msg 
			<< '\n';

	}
	inline void LogInit(InitStatus stat, const char* msg) {
		std::cout << "[" << ToColor(stat) << ToString(stat) << cmdcolor::reset << "] " << msg << '\n';
	}

private:

	SeverityMask g_severity = LogSeverity::FATAL | LogSeverity::DEBUG | LogSeverity::WARN | LogSeverity::ERROR;

	inline std::string Center(const std::string& s, size_t width) {
		if (s.size() >= width) return s;
		size_t padding = (width - s.size()) / 2;
		size_t padding_right = width - padding - s.size();
		return std::string(padding, ' ') + s + std::string(padding_right, ' ');
	}

	inline std::string CenterCustom(const std::string& s, size_t left_width, size_t right_width) {
		if (s.size() >= right_width) return s;
		size_t right_pad = right_width - s.size();
		return std::string(left_width, ' ') + s + std::string(right_pad, ' ');
	}



	inline void OutputTime() {

		auto now = std::chrono::system_clock::now();

		std::time_t now_t = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm;

		localtime_s(&now_tm, &now_t);

		char buff[80];
		strftime(buff, sizeof(buff), "%T", &now_tm);

		std::cout << "[" << buff << "]";

	}

	inline std::string ToString(LogSeverity sev) {
		switch (sev) {
		case LogSeverity::FATAL: return "FATAL";
		case LogSeverity::ERROR: return "ERROR";
		case LogSeverity::WARN:  return "WARN";
		case LogSeverity::INFO:  return "INFO";
		case LogSeverity::DEBUG: return "DEBUG";
		}
		return "";
	}
	inline std::string ToColor(LogSeverity sev) {
		switch (sev) {
		case LogSeverity::FATAL: return cmdcolor::magenta;
		case LogSeverity::ERROR: return cmdcolor::red;
		case LogSeverity::WARN:  return cmdcolor::yellow;
		case LogSeverity::INFO:  return cmdcolor::green;
		case LogSeverity::DEBUG: return cmdcolor::blue;
		}
		return "";
	}
	inline std::string ToString(InitStatus stat) {
		switch (stat) {
		case InitStatus::OK:	return "   OK   ";
		case InitStatus::FAIL:	return "  FAIL  ";
		}
		return "";
	}
	inline std::string ToColor(InitStatus stat) {
		switch (stat) {
		case InitStatus::OK:	return cmdcolor::green;
		case InitStatus::FAIL:	return cmdcolor::red;
		}
		return "";
	}

	Logger() = default;

};