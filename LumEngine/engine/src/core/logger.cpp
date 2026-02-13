#include "core/utils/logger.hpp"
#include "core/core_common.hpp"
namespace lum {

	void Logger::output_time() {

		auto now = std::chrono::system_clock::now();

		std::time_t now_t = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm;

		localtime_s(&now_tm, &now_t);

		char buff[64];
		strftime(buff, sizeof(buff), "%T", &now_tm);

		printf("[%s]", buff);

	}

	void Logger::to_string(charptr out, LogSeverity sev) {
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

	void Logger::to_color(charptr out, LogSeverity sev) {
		switch (sev) {
		case LogSeverity::Fatal: {
			constexpr usize len = sizeof(cmdcolor::magenta) - 1;
			std::memcpy(out, cmdcolor::magenta, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::Error: {
			constexpr usize len = sizeof(cmdcolor::gRed) - 1;
			std::memcpy(out, cmdcolor::gRed, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::Warn: {
			constexpr usize len = sizeof(cmdcolor::gYellow) - 1;
			std::memcpy(out, cmdcolor::gYellow, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::Info: {
			constexpr usize len = sizeof(cmdcolor::gGreen) - 1;
			std::memcpy(out, cmdcolor::gGreen, len);
			out[len] = '\0';
			break;
		}
		case LogSeverity::Debug: {
			constexpr usize len = sizeof(cmdcolor::gBlue) - 1;
			std::memcpy(out, cmdcolor::cyan, len);
			out[len] = '\0';
			break;
		}
		}
	}


}