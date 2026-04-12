#include "core/file_explorer.hpp"

namespace lum::editor {

	void FileExplorer::Update( StringView root ) {

        ImGui::Begin( "File Explorer" );
        DrawDirectory( root );
        ImGui::End( );

	}
	void FileExplorer::DrawDirectory( const std::filesystem::path& path ) {
        for (const auto& entry : std::filesystem::directory_iterator( path )) {
            if (entry.is_directory( )) {
                if (ImGui::TreeNode( entry.path( ).filename( ).string( ).c_str( ) )) {
                    DrawDirectory( entry.path( ) );
                    ImGui::TreePop( );
                }
            }
            else {
                ImGui::TreeNodeEx(
                    entry.path( ).filename( ).string( ).c_str( ),
                    ImGuiTreeNodeFlags_Leaf |
                    ImGuiTreeNodeFlags_NoTreePushOnOpen |
                    ImGuiTreeNodeFlags_SpanAvailWidth
                );
            }
        }
    }

}