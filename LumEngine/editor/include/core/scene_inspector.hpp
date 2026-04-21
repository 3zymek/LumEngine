#pragma once

#include "core/editor_common.hpp"
#include "entity/ecs_common.hpp"
#include "core/utils/fixed_string.hpp"
#include "core/utils/timed_tooltip.hpp"
namespace lum::editor {

	class SceneInspector {
	public:

		void Update( FScene* scene );
		EntityID GetSelectedEntity( ) const noexcept { return mSelectedEntity; }

	private:
		
		bool bEntityCreatorOpened = false;
		bool bJustOpened = true;

		EntityID mEntityToCreation;

		FixedString<64> mEntitiesFilter;
		FixedString<32> mComponentsFilter;
		EntityID mSelectedEntity = ecs::skNullEntity;
		const EditorComponentEntry* mSelectedComponent = nullptr;

		TimedTooltip mActionTooltip;

		void handle_entity_creator( FScene* scene );

	};

}