#include "core/scene_inspector.hpp"
#include "scene/scene_manager.hpp"
#include "entity/components/name.hpp"

namespace lum::editor {

	void SceneInspector::Update( FScene* scene ) {
		
		static uint32 sIndices{};
		sIndices = 0;

		auto& entities = scene->mEntities;

		ImGui::Begin( "Scene Inspector" );
		for (auto& entity : entities) {
			sIndices++;

			bool selected = (mSelectedEntity == entity);
			CName* name = scene->mEntityMgr.GetComponent<CName>( entity );
			String label;
			if (!name) label = "Entity " + ToString( entity );
			else label = name->mName.Data( );

			ImVec4 backgroundCol =
				(sIndices % 2 != 0)
				? ImVec4( 0.12f, 0.12f, 0.12f, 1.0f )
				: ImVec4( 0.09f, 0.09f, 0.09f, 1.0f );

			ImVec2 pos = ImGui::GetCursorScreenPos( );
			ImGui::SetCursorScreenPos( ImVec2( pos.x + 8.0f, pos.y ) );
			float32 width = ImGui::GetContentRegionAvail( ).x;
			float32 height = ImGui::GetTextLineHeightWithSpacing( );
			ImGui::GetWindowDrawList( )->AddRectFilled(
				pos,
				ImVec2( pos.x + width, pos.y + height ),
				ImGui::ColorConvertFloat4ToU32( backgroundCol ),
				4.0f
			);

			ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0.25f, 0.25f, 0.25f, 1.0f ) );
			ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0.18f, 0.18f, 0.18f, 1.0f ) );
			ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0.28f, 0.28f, 0.28f, 1.0f ) );

			if (ImGui::Selectable( label.data( ), selected, ImGuiSelectableFlags_None, ImVec2( 0, height ) )) {
				mSelectedEntity = entity;
			}

			ImGui::PopStyleColor( 3 );

		}
		ImGui::End( ); // Scene Inspector

	}

}