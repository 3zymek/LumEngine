#include "core/file_explorer.hpp"

namespace lum::editor {

	void FileExplorer::Update( StringView root ) {

		ImGui::Begin( "File Explorer" );
		//ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		//ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		//ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );

		DrawDirectory( root );
		//ImGui::PopStyleColor( 3 );
		ImGui::End( );

	}
	void FileExplorer::DrawDirectory( const std::filesystem::path& path ) {
		for (const auto& entry : std::filesystem::directory_iterator( path )) {
			if (entry.is_directory( )) {
				if (ImGui::TreeNodeEx( entry.path( ).filename( ).string( ).c_str( ), ImGuiTreeNodeFlags_SpanFullWidth )) {
					DrawDirectory( entry.path( ) );
					ImGui::TreePop( );
				}
			}
			else {
				
				float32 indent = ImGui::GetTreeNodeToLabelSpacing( );
				ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + indent );

				if (ImGui::Selectable( entry.path( ).filename( ).string( ).c_str( ) )) {

				}
			}
		}
	}

}