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
		case LogSeverity::FATAL: {
			std::memcpy(out, "FATAL\0", 5);
			break;
		}
		case LogSeverity::ERROR: {
			std::memcpy(out, "ERROR\0", 6);
			break;
		}
		case LogSeverity::WARN: {
			std::memcpy(out, "WARN\0", 5);
			break;
		}
		case LogSeverity::INFO: {
			std::memcpy(out, "INFO\0", 5);
			break;
		}
		case LogSeverity::DEBUG: {
			std::memcpy(out, "DEBUG\0", 6);
			break;
		}
		}
	}

	void Logger::_ToColor(charptr out, LogSeverity sev) {
		switch (sev) {
		case LogSeverity::FATAL: {
			constexpr usize len = sizeof(cmdcolor::magenta) - 1;
			std::memcpy(out, cmdcolor::magenta, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::ERROR: {
			constexpr usize len = sizeof(cmdcolor::red) - 1;
			std::memcpy(out, cmdcolor::red, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::WARN: {
			constexpr usize len = sizeof(cmdcolor::yellow) - 1;
			std::memcpy(out, cmdcolor::yellow, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::INFO: {
			constexpr usize len = sizeof(cmdcolor::green) - 1;
			std::memcpy(out, cmdcolor::green, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::DEBUG: {
			constexpr usize len = sizeof(cmdcolor::blue) - 1;
			std::memcpy(out, cmdcolor::blue, len);
			out[len] = '\0';
			break;
		}
		}
	}


}