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

		for (const auto& entry : std::filesystem::directory_iterator( path )) {

			const bool isDir = entry.is_directory( );
			const String name = entry.path( ).filename( ).string( );

			if (mFileFilter.Length( ) > 0 && !isDir) {
				const auto it = std::search(
					name.begin( ), name.end( ),
					mFileFilter.begin( ), mFileFilter.end( ),
					[]( char a, char b ) { return tolower( a ) == tolower( b ); }
				);
				if (it == name.end( )) continue;
			}

			FileIconInfo iconInfo = GetFileIcon( path, isDir );
			const ImU32 iconColor = ImGui::ColorConvertFloat4ToU32( iconInfo.mColor );
			const float32 iconWidth = ImGui::CalcTextSize( iconInfo.mIcon ).x + 4.f;
			const float32 itemHeight = ImGui::GetTextLineHeightWithSpacing( );
			const ImVec2 pos = ImGui::GetCursorScreenPos( );
			const float32 width = ImGui::GetContentRegionAvail( ).x;
			const bool hovered = ImGui::IsMouseHoveringRect( pos, ImVec2( pos.x + width, pos.y + itemHeight ) );
			const bool selected = (mSelectedPath == entry.path( ));

			if (selected)
				DrawRowBackground( style::skItemSelected );
			else if (hovered)
				DrawRowBackground( style::skItemHovered );

			ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 12.0f, 4.0f ) );

			if (isDir) {

				ImVec2 iconPos = ImGui::GetCursorScreenPos( );
				iconPos.y += (itemHeight - ImGui::GetTextLineHeight( )) * 0.5f;
				ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + iconWidth );

				bool open = ImGui::TreeNodeEx( name.c_str( ),
											   ImGuiTreeNodeFlags_SpanFullWidth |
											   ImGuiTreeNodeFlags_FramePadding
				);

				dl->AddText( iconPos, iconColor, iconInfo.mIcon );

				if (selected) {
					dl->AddRectFilled(
						ImVec2( pos.x + 2.f, pos.y + 2.f ),
						ImVec2( pos.x + 5.f, pos.y + 16.5f ),
						ImGui::ColorConvertFloat4ToU32( style::skAccentActive ),
						1.f
					);
				}

				if (ImGui::IsItemClicked( ))
					mSelectedPath = entry.path( );

				ImGui::PopStyleVar( );
				ImGui::PopStyleColor( 3 );

				if (open) {
					draw_directory( entry.path( ) );
					ImGui::TreePop( );
				}

			}
			else {

				const float32 indent = ImGui::GetTreeNodeToLabelSpacing( );
				ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + (itemHeight - ImGui::GetTextLineHeight( )) * 0.5f );
				ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + indent + iconWidth );

				if (ImGui::Selectable( name.c_str( ), selected, ImGuiSelectableFlags_None, ImVec2( 0, itemHeight ) ))
					mSelectedPath = entry.path( );

				ImVec2 iconPos = ImGui::GetItemRectMin( );
				iconPos.x -= iconWidth;
				iconPos.y += (ImGui::GetItemRectSize( ).y - ImGui::GetTextLineHeight( )) * 0.5f;
				dl->AddText( iconPos, iconColor, iconInfo.mIcon );

				if (selected) {
					dl->AddRectFilled(
						ImVec2( pos.x, pos.y ),
						ImVec2( pos.x + 5.f, pos.y + 16.5f ),
						ImGui::ColorConvertFloat4ToU32( style::skAccentActive ),
						1.f
					);
				}

				ImGui::PopStyleVar( );
				ImGui::PopStyleColor( 3 );

			}
		}

	}

} // namespace lum::editor