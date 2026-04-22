#include "core/scene_inspector.hpp"
#include "scene/scene_manager.hpp"
#include "entity/components/name.hpp"
#include "core/utils/style.hpp"
#include "core/editor_common.hpp"
namespace lum::editor {

	void SceneInspector::Update( FScene* scene ) {

		auto& entities = scene->mEntities;

		ImGui::Begin( "Scene" );

		if (ImGui::Button( ICON_FA_PLUS )) {
			mEntityCreator.Toggle( true );
		}
		ImGui::SameLine( );

		ImGui::PushItemWidth( -1 );
		SearchField( "##Entities search", "Filter entities...", mEntitiesFilter.Data( ), mEntitiesFilter.MaxSize( ) );
		ImGui::PopItemWidth( );

		ImGui::Separator( );

		for (usize i = 0; i < entities.size( ); i++) {

			auto& entity = entities[ i ];

			bool selected = (mSelectedEntity == entity);
			CName* name = scene->mEntityMgr.GetComponent<CName>( entity );
			char label[ 64 ]{};
			if (!name || name->mName.Length( ) == 0)
				FormatString( label, "%s %llu", "Entity", entity );
			else
				FormatString( label, "%s", name->mName.Data( ) );

			const usize len = strlen( label );
			auto it = std::search(
				label, label + len,
				mEntitiesFilter.begin( ), mEntitiesFilter.end( ),
				[]( char a, char b ) { return tolower( a ) == tolower( b ); }
			);

			if (it == label + len) continue;

			float32 height = ImGui::GetTextLineHeightWithSpacing( );
			ImVec2 pos = ImGui::GetCursorScreenPos( );
			float32 width = ImGui::GetContentRegionAvail( ).x;
			bool hovered = ImGui::IsMouseHoveringRect( pos, ImVec2( pos.x + width, pos.y + height ) );

			if (entity == mSelectedEntity)
				DrawRowBackground( style::skItemSelected );
			else if (hovered)
				DrawRowBackground( style::skItemHovered );
			else
				DrawRowBackground(
					(i % 2 != 0)
					? style::skRowAlt
					: style::skBg
				);

			ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 12.0f, 4.0f ) );

			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + (height - ImGui::GetTextLineHeight( )) * 0.5f );

			if (ImGui::Selectable( label, selected, ImGuiSelectableFlags_None, ImVec2( 0, height ) )) {
				mSelectedEntity = entity;
			}
			if (selected) {
				ImVec2 min = ImGui::GetItemRectMin( );
				ImVec2 max = ImGui::GetItemRectMax( );
				ImGui::GetWindowDrawList( )->AddRectFilled(
					ImVec2( pos.x + 2.0f, pos.y + 2.0f ),
					ImVec2( pos.x + 5.0f, pos.y + 16.5f ),
					ImGui::ColorConvertFloat4ToU32( style::skAccentActive ),
					1.0f
				);
			}

			ImGui::PopStyleVar( );
			ImGui::PopStyleColor( 3 );

		}
		ImGui::End( ); // Scene Inspector

		mEntityCreator.Handle( scene );

		mActionTooltip.Draw( );

	}

} // namespace lum::editor