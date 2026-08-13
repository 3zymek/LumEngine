#include "Core/FileExplorer.hpp"
#include "Core/Utils/Style.hpp"
#include "Core/EditorSelection.hpp"
#include "Platform/FileSystem/Directory.hpp"

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

	void FileExplorer::draw_directory( const Path& path ) {

		ImDrawList* dl = ImGui::GetWindowDrawList( );

		int32 iteration = 0;
		char buffer[ 256 ];
		for (auto entry : Directory( path )) {

			const bool isDir = entry.IsDirectory( );
			FileIconInfo iconInfo = GetFileIcon( entry );

			FormatString( buffer, "%s %s", iconInfo.mIcon, entry.Filename( ).ToString().c_str() );
			if (isDir) {

				const bool selected = mSelectedPath == entry;
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

				if (ImGui::IsItemClicked( ) && !ImGui::IsItemToggledOpen( )) {
					mSelectedPath = entry;
					EditorSelection::SelectFile( mSelectedPath );
				}

				ImGui::PopStyleColor( 3 );

				if (open) {
					draw_directory( entry );
					ImGui::TreePop( );
				}

			}
			else {

				float32 indent = ImGui::GetTreeNodeToLabelSpacing( );
				ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + indent );

				const bool selected = mSelectedPath == entry ? true : false;
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
					mSelectedPath = entry;
					EditorSelection::SelectFile( mSelectedPath );
				}
				if (ImGui::BeginDragDropSource( )) {

					const String& path = entry.ToString( );
					ImGui::SetDragDropPayload( "FILE", path.data(), path.size() + 1 );
					ImGui::Text( entry.Filename( ).ToString().c_str() );
					ImGui::EndDragDropSource( );

				}
				ImGui::PopStyleColor( 3 );
				DrawPopupContext( {
					.mItems = {
						{ ICON_FA_TRASH,       "Delete", "Del",    [&]( ) { }},
						{ ICON_FA_PEN,         "Rename", "F2",     [&]( ) { }},
						{ ICON_FA_COPY,        "Copy",   "Ctrl+C", [&]( ) { }},
					}
					} 
				);

			}

			iteration++;

		}

	}

} // namespace lum::editor