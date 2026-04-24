//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI panel for inspecting and editing components of a selected entity.
//
//=============================================================================//

#include "core/entity_inspector.hpp"
#include "editor.hpp"

namespace lum::editor {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void EntityInspector::Handle( EntityID selectedEntity, FScene* scene ) {

		ImGui::Begin( "Entity" );
		if (ImGui::Button( ICON_FA_PENCIL )) {
			mEntityEditor.Toggle( true );
		} TooltipOnHover( "Edit entity" );
		ImGui::SameLine( );
		ImGui::SetNextItemWidth( -1 );
		SearchField( "##ComponentsSearchField", "Filter fields...", mFieldsFilter.Data( ), mFieldsFilter.MaxSize( ) );

		ImGui::Separator( );

		if (selectedEntity != ecs::skNullEntity) {
			FCollapsingHeaderArgs args;
			scene->mEntityMgr.ForEachComponent(
				selectedEntity,
				[&]( int32 typeID, ecs::BasePool* pool ) {

					bool opened = true;

					args.mFont = Fonts::sDefaultMedium;
					args.mID = HashStr( pool->GetParseName( ) );
					args.mLabel = pool->GetDisplayName( );
					CollapsingHeaderCustom( args, opened );

					if (opened) {
						const ComponentsEntriesMap& map = Editor::GetComponentsEntries( );
						auto it = map.find( HashStr( pool->GetParseName( ) ) );
						if (it != map.end( )) {
							it->second.mDisplayFn( scene->mEntityMgr, selectedEntity );
						}
					}


				} );

			mEntityEditor.Handle( scene, selectedEntity );

		}
		ImGui::End( );

	}

}