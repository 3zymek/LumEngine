#pragma once

#include "core/editor_common.hpp"
#include "entity/ecs_common.hpp"
#include "core/utils/fixed_string.hpp"
#include "core/utils/timed_tooltip.hpp"
#include "core/entity_creator.hpp"
namespace lum::editor {

	class SceneInspector {
	public:

		void Update( FScene* scene );
		EntityID GetSelectedEntity( ) const noexcept { return mSelectedEntity; }

	private:

		FixedString<64> mEntitiesFilter;
		FixedString<32> mComponentsFilter;
		EntityID mSelectedEntity = ecs::skNullEntity;

		TimedTooltip mActionTooltip;

		EntityCreator mEntityCreator;

	};

}