//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Editor console for logging, filtering and log inspection.
//
//=============================================================================//
#pragma once

#include "core/utils/logger.hpp"
#include "core/core_common.hpp"
#include "core/utils/timed_tooltip.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum::editor {

	// In-engine console used for displaying and interacting with logs.
	class Console {
	public:

		Console( ) {
			LUM_LOG_INFO( "LumEngine v0.1.0.dev (c) 2025-present 3zymek" );
		}

		// Logs an info message through global logger.
		template<typename... tArgs>
		void Log( StringView msg, tArgs&&... args ) const noexcept {
			LUM_LOG_INFO( msg.data( ), std::forward<tArgs>( args )... );
		}

		// Logs a warning message through global logger.
		template<typename... tArgs>
		void Warn( StringView msg, tArgs&&... args ) const noexcept {
			LUM_LOG_WARN( msg.data( ), std::forward<tArgs>( args )... );
		}

		// Logs an error message through global logger.
		template<typename... tArgs>
		void Error( StringView msg, tArgs&&... args ) const noexcept {
			LUM_LOG_ERROR( msg.data( ), std::forward<tArgs>( args )... );
		}

		// Updates and renders console UI.
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

		// Draws timestamp prefix for a log entry.
		void draw_timestamp( const FLogEntry& log );

		// Draws left sidebar with severity filters and counters.
		void draw_sidebar( );

		// Handles text filtering for log messages.
		void handle_word_filtering( );

		// Handles clearing logs action.
		void handle_eraser( );

		// Handles copying selected logs to clipboard.
		void handle_copy( );

		// Handles search/filter UI logic.
		void handle_search( );

		// Scoped RAII style helper for action buttons in console UI.
		struct ScopedActionButtonStyle {
			ScopedActionButtonStyle( ) {
				ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0, 0, 0, 0 ) );
				ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.2f, 0.2f, 0.2f, 1.0f ) );
				ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.3f, 0.3f, 0.3f, 1.0f ) );
				ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );
			}

			~ScopedActionButtonStyle( ) {
				ImGui::PopStyleVar( );
				ImGui::PopStyleColor( 3 );
			}
		};

		// Handles rendering of a filter button for given severity.
		void handle_filter( StringView icon, uint32 numLogs, LogSeverity sev );
	};

} // namespace lum::editor