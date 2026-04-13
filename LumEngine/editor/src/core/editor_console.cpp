#include "core/editor_console.hpp"
#include "core/utils/fonts.hpp"

namespace lum::editor {

	void Console::Draw( ) {

		mNumDebugLogs = 0;
		mNumInfoLogs = 0;
		mNumWarnLogs = 0;
		mNumErrorLogs = 0;

		ImGui::Begin( "Console" );
		ImGuiDockNode* node = ImGui::GetWindowDockNode( );
		if (node) {
			node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
		}
		ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 6.0f );
		ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 0.1f );
		ImGui::BeginChild( "##LumEngineConsole", ImVec2( -68, 0 ), true );
		ImGui::PopStyleVar( 2 );
		ImGui::Indent( 8.0f );
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 4.0f );
		ImGui::PushFont( Fonts::sConsole );
		auto& logs = Logger::Get( ).GetLogs( );
		for (auto& log : logs) {

			if (bDrawTime)
				draw_timestamp( log );

			if (log.mSeverity == LogSeverity::Debug)
				mNumDebugLogs++;
			else if (log.mSeverity == LogSeverity::Info)
				mNumInfoLogs++;
			else if (log.mSeverity == LogSeverity::Warn)
				mNumWarnLogs++;
			else if (log.mSeverity == LogSeverity::Error)
				mNumErrorLogs++;

			ImVec4 color = sSeverityColors[ ToUnderlyingEnum( log.mSeverity ) ];
			ImGui::TextColored( color, log.mMessage.data( ) );

		}
		ImGui::PopFont( );
		ImGui::Unindent( 8.0f );
		ImGui::EndChild( );

		ImGui::SameLine( );

		draw_sidebar( );

		ImGui::End( );

	}

	void Console::draw_timestamp( const FLogEntry& log ) {

		char time[ 10 ]{};
		Logger::FormatTime( log.mTime, time );

		ImGui::TextDisabled( "[ " ); ImGui::SameLine( );
		ImGui::TextDisabled( time ); ImGui::SameLine( );
		ImGui::TextDisabled( " ]" ); ImGui::SameLine( );

	}
	void Console::draw_sidebar( ) {
		
		ImGui::BeginChild( "##ConsoleSidebar", ImVec2( 60, 0 ), false, ImGuiWindowFlags_NoBackground );

		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0, 0, 0, 0 ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.2f, 0.2f, 0.2f, 1.0f ) );

		handle_easer( );
		ImGui::SameLine( );
		handle_copy( );

		ImGui::PopStyleColor( 2 );

		ImGui::Separator( );
		handle_info_filter( );

		ImGui::EndChild( ); // ##ConsoleSidebar
			
	}
	void Console::handle_easer( ) {
		
		if (ImGui::Button( ICON_FA_ERASER )) { Logger::Get( ).ClearLogs( ); }

	}
	void Console::handle_copy( ) {

		if (ImGui::Button( ICON_FA_CLONE )) {

		}

	}
	void Console::handle_debug_filter( ) {

		char buff[ 32 ];
		snprintf( buff, sizeof( buff ), "%s, %d", ICON_FA_INFO_CIRCLE, mNumInfoLogs );

		if (ImGui::Button( buff, ImVec2( 55, 30 ) )) {

		}

	}
	void Console::handle_info_filter( ) {

	}
	void Console::handle_warn_filter( ) {

	}
	void Console::handle_error_filter( ) {

	}

}