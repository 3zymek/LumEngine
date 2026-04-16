#include "core/scene_inspector.hpp"
#include "scene/scene_manager.hpp"
#include "entity/components/name.hpp"
#include "core/utils/style.hpp"
#include "core/utils/fonts.hpp"

namespace lum::editor {

	void SceneInspector::Update( FScene* scene ) {

		auto& entities = scene->mEntities;

		ImGui::Begin( "Scene Inspector" );

		if (ImGui::Button( ICON_FA_PLUS )) {

		} 
		ImGui::SameLine( );
		
		ImGui::PushItemWidth( -1 );
		SearchField( "##Entities search", "Filter entities...", mEntitiesFilter.Data( ), mEntitiesFilter.MaxSize( ) );
		ImGui::PopItemWidth();

		ImGui::Separator( );

		for (usize i = 0; i < entities.size( ); i++) {

			auto& entity = entities[ i ];

			bool selected = (mSelectedEntity == entity);
			CName* name = scene->mEntityMgr.GetComponent<CName>( entity );
			char label[ 64 ]{};
			if (!name) snprintf( label, sizeof( label ), "%s %d", "Entity", entity );
			else snprintf( label, sizeof( label ), "%s", name->mName.Data( ) );

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
					? style::skBgPopup
					: style::skBg
				);

			ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0, 0, 0, 0 ) );

			if (ImGui::Selectable( label, selected, ImGuiSelectableFlags_None, ImVec2( 0, height ) )) {
				mSelectedEntity = entity;
			}

			ImGui::PopStyleColor( 3 );

		}
		ImGui::End( ); // Scene Inspector

	}

}