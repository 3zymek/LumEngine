#pragma once

#include "core/utils/logger.hpp"

namespace lum::editor {

	class Console {
	public:

		template<typename ...tArgs>
		void Log( StringView msg, tArgs&&... args ) const noexcept { LUM_LOG_INFO( msg.data( ), std::forward<tArgs>( args )... ); }

		template<typename ...tArgs>
		void Warn( StringView msg, tArgs&&... args ) const noexcept { LUM_LOG_WARN( msg.data( ), std::forward<tArgs>( args )... ); }

		template<typename ...tArgs>
		void Error( StringView msg, tArgs&&... args ) const noexcept { LUM_LOG_ERROR( msg.data( ), std::forward<tArgs>( args )... ); }

		template<typename ...tArgs>
		void Fatal( StringView msg, tArgs&&... args ) const noexcept { LUM_LOG_FATAL( msg.data( ), std::forward<tArgs>( args )... ); }

		void Draw( );


	private:

		inline static std::unordered_map<SeverityMask, ImVec4> sSeverityColors = {
			{ ToUnderlyingEnum( LogSeverity::Debug ), ImVec4( 0.55f, 0.55f, 0.60f, 1.0f ) },
			{ ToUnderlyingEnum( LogSeverity::Info ),  ImVec4( 0.85f, 0.85f, 0.85f, 1.0f ) },
			{ ToUnderlyingEnum( LogSeverity::Warn ),  ImVec4( 0.90f, 0.70f, 0.20f, 1.0f ) },
			{ ToUnderlyingEnum( LogSeverity::Error ), ImVec4( 0.90f, 0.30f, 0.30f, 1.0f ) },
			{ ToUnderlyingEnum( LogSeverity::Fatal ), ImVec4( 0.85f, 0.25f, 0.70f, 1.0f ) }
		};


	};

}