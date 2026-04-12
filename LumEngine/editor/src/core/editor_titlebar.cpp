
#include "core/editor_titlebar.hpp"

namespace lum::editor {

	void WindowTitlebar::Draw( Window* window ) {

		

		ImGui::SetNextWindowPos( ImVec2( 0, 0 ) );
		ImGui::SetNextWindowSize( ImVec2( ) );

		ImGui::Begin( "##LumEngineWindowTitlebar", 
					  nullptr, 
					  ImGuiWindowFlags_NoTitleBar |
					  ImGuiWindowFlags_NoResize |
					  ImGuiWindowFlags_NoScrollbar 
		);




		ImGui::End( );

	}

}