#include "core/logger.hpp"
#include "core/core_common.hpp"
namespace lum {

	void Logger::_OutputTime() {

		auto now = std::chrono::system_clock::now();

		std::time_t now_t = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm;

		localtime_s(&now_tm, &now_t);

		char buff[64];
		strftime(buff, sizeof(buff), "%T", &now_tm);

		printf("[%s]", buff);

	}

	void Logger::_ToString(charptr out, LogSeverity sev) {
		switch (sev) {
		case LogSeverity::Fatal: {
			std::memcpy(out, "FATAL\0", 5);
			break;
		}
		case LogSeverity::Error: {
			std::memcpy(out, "ERROR\0", 6);
			break;
		}
		case LogSeverity::Warn: {
			std::memcpy(out, "WARN\0", 5);
			break;
		}
		case LogSeverity::Info: {
			std::memcpy(out, "INFO\0", 5);
			break;
		}
		case LogSeverity::Debug: {
			std::memcpy(out, "DEBUG\0", 6);
			break;
		}
		}
	}

	void Logger::_ToColor(charptr out, LogSeverity sev) {
		switch (sev) {
		case LogSeverity::Fatal: {
			constexpr usize len = sizeof(cmdcolor::magenta) - 1;
			std::memcpy(out, cmdcolor::magenta, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::Error: {
			constexpr usize len = sizeof(cmdcolor::red) - 1;
			std::memcpy(out, cmdcolor::red, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::Warn: {
			constexpr usize len = sizeof(cmdcolor::yellow) - 1;
			std::memcpy(out, cmdcolor::yellow, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::Info: {
			constexpr usize len = sizeof(cmdcolor::green) - 1;
			std::memcpy(out, cmdcolor::green, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::Debug: {
			constexpr usize len = sizeof(cmdcolor::blue) - 1;
			std::memcpy(out, cmdcolor::cyan, len);
			out[len] = '\0';
			break;
		}
		}
	}


}