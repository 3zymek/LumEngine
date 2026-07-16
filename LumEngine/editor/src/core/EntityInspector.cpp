//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI panel for inspecting and editing components of a selected entity.
//
//=============================================================================//

#include "Core/EntityInspector.hpp"
#include "Editor.hpp"

namespace lum::editor {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void EntityInspector::Update( EntityID selectedEntity, Scene* scene ) {

		ImGui::Begin( "Entity" );
		if (ImGui::Button( ICON_FA_PEN )) {
			mEntityEditor.Toggle( true );
		} TooltipOnHover( "Edit entity" );
		ImGui::SameLine( );
		ImGui::SetNextItemWidth( -1 );
		SearchField( "##ComponentsSearchField", "Filter fields...", mFieldsFilter.Data( ), mFieldsFilter.MaxSize( ) );

		ImGui::Separator( );

		if (selectedEntity != ecs::kNullEntity) {
			CollapsingHeaderArgs args;
			scene->mEntityMgr.ForEachComponent(
				selectedEntity,
				[&]( int32 typeID, ecs::ComponentBasePool* pool ) {

					bool opened = true;

					args.mFont = Fonts::sDefaultMedium;
					args.mID = HashString( pool->GetParseName( ) );
					args.mLabel = pool->GetDisplayName( );
					CollapsingHeaderCustom( args, opened );

					if (opened) {
						const ComponentsEntriesMap& map = Editor::GetComponentsEntries( );
						auto it = map.find( HashString( pool->GetParseName( ) ) );
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