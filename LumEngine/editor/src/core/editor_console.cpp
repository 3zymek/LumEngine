#include "core/editor_console.hpp"
#include "core/utils/fonts.hpp"

namespace lum::editor {

	void Console::Draw( ) {

		ImGui::Begin( "Console" );
		ImGuiDockNode* node = ImGui::GetWindowDockNode( );
		if (node) {
			node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
		}
		ImGui::BeginChild( "##LumEngineConsole", ImVec2( 0, 0 ) );
		ImGui::PushFont( Fonts::sConsole );
		auto& logs = Logger::Get( ).GetLogs( );
		for (auto& log : logs) {

			char time[ 10 ]{};
			Logger::FormatTime( log.mTime, time );

			ImGui::TextDisabled( "[ " ); ImGui::SameLine( );
			ImGui::TextDisabled( time ); ImGui::SameLine( );
			ImGui::TextDisabled( " ]" ); ImGui::SameLine( );

			ImVec4 color = sSeverityColors[ ToUnderlyingEnum( log.mSeverity ) ];
			ImGui::TextColored( color, log.mMessage.data( ) );

		}
		ImGui::PopFont( );
		ImGui::EndChild( );
		ImGui::End( );

	}

}