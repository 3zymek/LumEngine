#include "core/file_explorer.hpp"

namespace lum::editor {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void FileExplorer::Update( StringView root ) {

		ImGui::Begin( "File System" );
		ImGui::SetNextItemWidth( -1 );
		SearchField( "##FileFilter", "Filter files...", mFileFilter.Data( ), mFileFilter.MaxSize( ) );
		ImGui::Separator( );
		draw_directory( root );
		ImGui::End( );

	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void FileExplorer::draw_directory( const std::filesystem::path& path ) {
		for (const auto& entry : std::filesystem::directory_iterator( path )) {
			if (entry.is_directory( )) {
				if (ImGui::TreeNodeEx( entry.path( ).filename( ).string( ).c_str( ), ImGuiTreeNodeFlags_SpanFullWidth )) {
					draw_directory( entry.path( ) );
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