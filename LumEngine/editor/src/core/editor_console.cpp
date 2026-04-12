#include "core/editor_console.hpp"
#include "core/utils/fonts.hpp"

namespace lum::editor {

	void Console::Draw( ) {

		ImGui::Begin( "Console" );
		ImGuiDockNode* node = ImGui::GetWindowDockNode( );
		if (node) {
			node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
		}
		ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 6.0f );
		ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 0.1f );
		ImGui::BeginChild( "##LumEngineConsole", ImVec2( 0, 0 ), true );
		ImGui::PopStyleVar( 2 );
		ImGui::Indent( 8.0f );
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 4.0f );
		ImGui::PushFont( Fonts::sConsole );
		auto& logs = Logger::Get( ).GetLogs( );
		for (auto& log : logs) {

			if (bDrawTime)
				draw_timestamp( log );

			ImVec4 color = sSeverityColors[ ToUnderlyingEnum( log.mSeverity ) ];
			ImGui::TextColored( color, log.mMessage.data( ) );

		}
		ImGui::PopFont( );
		ImGui::Unindent( 8.0f );
		ImGui::EndChild( );
		ImGui::End( );

	}

	void Console::draw_timestamp( const FLogEntry& log ) {

		char time[ 10 ]{};
		Logger::FormatTime( log.mTime, time );

		ImGui::TextDisabled( "[ " ); ImGui::SameLine( );
		ImGui::TextDisabled( time ); ImGui::SameLine( );
		ImGui::TextDisabled( " ]" ); ImGui::SameLine( );

	}

}