#include "core/entity_inspector.hpp"
#include "editor_dep_manager.generated.hpp"
#include "editor.hpp"

namespace lum::editor {

	void EntityInspector::Update( EntityID selectedEntity, FScene* scene ) {

		FCollapsingHeaderArgs args;
		ImGui::Begin( "Entity" );
		if (selectedEntity != ecs::skNullEntity) {
			scene->mEntityMgr.ForEachComponent(
				selectedEntity,
				[&]( ecs::detail::BasePool* pool ) {

					bool opened = true;

					args.mFont = Fonts::sDefaultMedium;
					args.mID = HashStr( pool->GetParseName() );
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
		}
		ImGui::End( );

	}

}