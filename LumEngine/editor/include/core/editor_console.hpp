#pragma once

#include "core/utils/logger.hpp"
#include "core/core_common.hpp"
#include "core/utils/timed_tooltip.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum::editor {

	class Console {
	public:

		Console( ) { LUM_LOG_INFO( "LumEngine v0.1.0.dev (c) 2025-present 3zymek" ); }

		template<typename ...tArgs>
		void Log( StringView msg, tArgs&&... args ) const noexcept { LUM_LOG_INFO( msg.data( ), std::forward<tArgs>( args )... ); }

		template<typename ...tArgs>
		void Warn( StringView msg, tArgs&&... args ) const noexcept { LUM_LOG_WARN( msg.data( ), std::forward<tArgs>( args )... ); }

		template<typename ...tArgs>
		void Error( StringView msg, tArgs&&... args ) const noexcept { LUM_LOG_ERROR( msg.data( ), std::forward<tArgs>( args )... ); }

		void Update( );


	private:

		static constexpr uint32 skSideBarWidth = 60;

		bool bFilterEnabled = false;
		bool bDrawTime = false;
		bool bClearLogs = false;
		FixedString<64> mFilter;
		uint32 mNumDebugLogs = 0;
		uint32 mNumInfoLogs = 0;
		uint32 mNumWarnLogs = 0;
		uint32 mNumErrorLogs = 0;

		TimedTooltip mActionTooltip;

		Flags<LogSeverity> mSeverity{ LogSeverity::All };

		inline static std::unordered_map<SeverityMask, ImVec4> sSeverityColors = {
			{ ToUnderlyingEnum( LogSeverity::Debug ), ImVec4( 0.55f, 0.55f, 0.60f, 1.0f ) },
			{ ToUnderlyingEnum( LogSeverity::Info ),  ImVec4( 0.85f, 0.85f, 0.85f, 1.0f ) },
			{ ToUnderlyingEnum( LogSeverity::Warn ),  ImVec4( 0.90f, 0.70f, 0.20f, 1.0f ) },
			{ ToUnderlyingEnum( LogSeverity::Error ), ImVec4( 0.90f, 0.30f, 0.30f, 1.0f ) },
			{ ToUnderlyingEnum( LogSeverity::Fatal ), ImVec4( 0.85f, 0.25f, 0.70f, 1.0f ) }
		};

		void draw_timestamp( const FLogEntry& log );
		void draw_sidebar( );
		void handle_word_filtering( );
		void handle_eraser( );
		void handle_copy( );
		void handle_search( );

		void handle_filter( StringView icon, uint32 numLogs, LogSeverity sev );
	};

}