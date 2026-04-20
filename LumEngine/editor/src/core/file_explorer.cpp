#include "core/file_explorer.hpp"

namespace lum::editor {

	void FileExplorer::Update( StringView root ) {

		ImGui::Begin( "File System" );
		DrawDirectory( root );
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