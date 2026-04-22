#include "core/entity_inspector.hpp"
#include "editor_dep_manager.generated.hpp"
#include "editor.hpp"

namespace lum::editor {

	void EntityInspector::Handle( EntityID selectedEntity, FScene* scene ) {

		ImGui::Begin( "Entity" );
		if (ImGui::Button( ICON_FA_PENCIL )) {
			mComponentsCreator.Toggle( true );
		}
		ImGui::SameLine( );
		ImGui::SetNextItemWidth( -1 );
		SearchField( "##ComponentsSearchField", "Filter fields...", mFieldsFilter.Data( ), mFieldsFilter.MaxSize( ) );

		ImGui::Separator( );

		if (selectedEntity != ecs::skNullEntity) {
			FCollapsingHeaderArgs args;
			scene->mEntityMgr.ForEachComponent(
				selectedEntity,
				[&]( ecs::detail::BasePool* pool ) {

					bool opened = true;

					args.mFont = Fonts::sDefaultMedium;
					args.mID = HashStr( pool->GetParseName( ) );
					args.mLabel = pool->GetDisplayName( );
					CollapsingHeaderCustom( args, opened );

					if (opened) {
						const ComponentsEntriesMap& map = Editor::GetComponentsEntries( );
						auto it = map.find( HashStr( pool->GetParseName( ) ) );
						if (it != map.end( )) {
							it->second.mEditorFn( scene->mEntityMgr, selectedEntity );
						}
					}


				} );

			mComponentsCreator.Handle( scene, selectedEntity );

		}
		ImGui::End( );

	}

}