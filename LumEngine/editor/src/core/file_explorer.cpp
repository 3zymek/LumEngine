#include "core/file_explorer.hpp"
#include "core/utils/style.hpp"

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
	//---------------------------------------------------------sa

	void FileExplorer::draw_directory( const std::filesystem::path& path ) {

		ImDrawList* dl = ImGui::GetWindowDrawList( );

		int32 iteration = 0;
		char buffer[ 256 ];
		for (const auto& entry : std::filesystem::directory_iterator( path )) {

			const std::filesystem::path entryPath = entry.path( );
			const bool isDir = entry.is_directory( );
			FileIconInfo iconInfo = GetFileIcon( entryPath, isDir );

			FormatString( buffer, "%s %s", iconInfo.mIcon, entryPath.filename( ).string( ).c_str( ) );
			if (isDir) {

				const bool selected = mSelectedPath == entryPath;
				const ImVec2 pos = ImGui::GetCursorScreenPos( );
				const float32 width = ImGui::GetContentRegionAvail( ).x;
				const float32 height = ImGui::GetTextLineHeightWithSpacing( );
				const bool hovered = ImGui::IsMouseHoveringRect( pos, ImVec2( pos.x + width, pos.y + height ) );

				if (selected)
					DrawRowBackground( style::skItemSelected );
				else if (hovered)
					DrawRowBackground( style::skItemHovered );
				else
					DrawRowBackground( style::skBg );

				ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0, 0, 0, 0 ) );
				ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0, 0, 0, 0 ) );
				ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0, 0, 0, 0 ) );

				bool open = ImGui::TreeNodeEx( buffer, ImGuiTreeNodeFlags_SpanFullWidth );

				if (ImGui::IsItemClicked( ) && !ImGui::IsItemToggledOpen( ))
					mSelectedPath = entryPath;

				ImGui::PopStyleColor( 3 );

				if (open) {
					draw_directory( entryPath );
					ImGui::TreePop( );
				}

			}
			else {

				float32 indent = ImGui::GetTreeNodeToLabelSpacing( );
				ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + indent );

				const bool selected = mSelectedPath == entryPath ? true : false;
				const ImVec2 pos = ImGui::GetCursorScreenPos( );
				const float32 width = ImGui::GetContentRegionAvail( ).x;
				const float32 height = ImGui::GetTextLineHeightWithSpacing( );
				const bool hovered = ImGui::IsMouseHoveringRect( pos, ImVec2( pos.x + width, pos.y + height ) );

				if (selected)
					DrawRowBackground( style::skItemSelected );
				else if (hovered)
					DrawRowBackground( style::skItemHovered );
				else
					DrawRowBackground( style::skBg );


				ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0, 0, 0, 0 ) );
				ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0, 0, 0, 0 ) );
				ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0, 0, 0, 0 ) );

				ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + (height - ImGui::GetTextLineHeight( )) * 0.5f );

				if (ImGui::Selectable( buffer, selected )) {
					mSelectedPath = entryPath;
				}
				ImGui::PopStyleColor( 3 );
				DrawPopupContext( {
					.mItems = {
						{ ICON_FA_TRASH,  "Delete", "Del",    [&]( ) {  }},
						{ ICON_FA_PENCIL, "Rename", "F2",     [&]( ) {  }},
						{ ICON_FA_FILES_O,"Copy",   "Ctrl+C", [&]( ) {  }},
					}
				} ); 

			}

			iteration++;

		}

	}

} // namespace lum::editor