#include "core/logger.hpp"
#include "core/core_common.hpp"
namespace lum {

	void Logger::Log(LogSeverity sev, const char* file, const char* func, int line, const std::string& msg) {
		if (!(g_severity & sev)) return;
		if (lastLog == hash(msg)) return;

		_OutputTime();

		std::filesystem::path p = file;
		std::string filename = p.filename().string();
		std::string severity = _ToString(sev);
		std::string info = filename + ":" + cmdcolor::cyan + std::to_string(line) + cmdcolor::reset + " " + func;

		std::cout
			<< "["
			<< _ToColor(sev)
			<< _CenterCustom(severity, 1, 6)
			<< cmdcolor::reset
			<< "]["
			<< _CenterCustom(info, 2, 52)
			<< "] "
			<< msg
			<< '\n';

		lastLog = hash(msg);

	}

	std::string Logger::_Center(const std::string& s, size_t width) {
		if (s.size() >= width) return s;
		size_t padding = (width - s.size()) / 2;
		size_t padding_right = width - padding - s.size();
		return std::string(padding, ' ') + s + std::string(padding_right, ' ');
	}

	std::string Logger::_CenterCustom(const std::string& s, size_t left_width, size_t right_width) {
		if (s.size() >= right_width) return s;
		size_t right_pad = right_width - s.size();
		return std::string(left_width, ' ') + s + std::string(right_pad, ' ');
	}

	void Logger::_OutputTime() {

		auto now = std::chrono::system_clock::now();

		std::time_t now_t = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm;

		localtime_s(&now_tm, &now_t);

		char buff[80];
		strftime(buff, sizeof(buff), "%T", &now_tm);

		std::cout << "[" << buff << "]";

	}

	std::string Logger::_ToString(LogSeverity sev) {
		switch (sev) {
		case LogSeverity::FATAL: return "FATAL";
		case LogSeverity::ERROR: return "ERROR";
		case LogSeverity::WARN:  return "WARN";
		case LogSeverity::INFO:  return "INFO";
		case LogSeverity::DEBUG: return "DEBUG";
		}
		return "";
	}

	std::string Logger::_ToColor(LogSeverity sev) {
		switch (sev) {
		case LogSeverity::FATAL: return cmdcolor::magenta;
		case LogSeverity::ERROR: return cmdcolor::red;
		case LogSeverity::WARN:  return cmdcolor::yellow;
		case LogSeverity::INFO:  return cmdcolor::green;
		case LogSeverity::DEBUG: return cmdcolor::blue;
		}
		return "";
	}

	std::string Logger::_ToString(InitStatus stat) {
		switch (stat) {
		case InitStatus::OK:	return "   OK   ";
		case InitStatus::FAIL:	return "  FAIL  ";
		}
		return "";
	}
	std::string Logger::_ToColor(InitStatus stat) {
		switch (stat) {
		case InitStatus::OK:	return cmdcolor::green;
		case InitStatus::FAIL:	return cmdcolor::red;
		}
		return "";
	}


}