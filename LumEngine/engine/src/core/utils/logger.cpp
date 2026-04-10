//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Logs messages form all over the engine for debugging and testing
// 
//=============================================================================//

#include "core/utils/logger.hpp"
#include "core/core_common.hpp"

namespace lum {

	StringView Logger::SeverityToString( LogSeverity sev ) {
		switch (sev) {
		case LogSeverity::Fatal:
		{
			return "FATAL";
			break;
		}
		case LogSeverity::Error:
		{
			return "ERROR";
			break;
		}
		case LogSeverity::Warn:
		{
			return "WARN";
			break;
		}
		case LogSeverity::Info:
		{
			return "INFO";
			break;
		}
		case LogSeverity::Debug:
		{
			return "DEBUG";
			break;
		}
		}
	}
	void Logger::FormatTime( uint64 timestamp, charptr out ) {
		std::time_t t = timestamp / 1000;
		std::tm tm{};

		localtime_s( &tm, &t );

		std::strftime( out, 16, "%H:%M:%S", &tm );
	}
	uint64 Logger::get_time( ) {
		using namespace std::chrono;

		return duration_cast< milliseconds >(
			system_clock::now( ).time_since_epoch( )
		).count( );
	}

}