//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI panel for browsing, selecting and managing scene entities.
//
//=============================================================================//
#pragma once

#include "core/editor_common.hpp"
#include "core/utils/fixed_string.hpp"
#include "core/utils/timed_tooltip.hpp"
#include "core/entity_creator.hpp"

namespace lum::editor {

	// UI panel for browsing, selecting and managing scene entities.
	class SceneInspector {
	public:

		// Updates and renders the scene inspector panel.
		void Update( FScene* scene );

		// Returns the currently selected entity, or skNullEntity if none.
		EntityID GetSelectedEntity( ) const noexcept { return mSelectedEntity; }

	private:

		// Filter string for the entity list search input.
		FixedString<64> mEntitiesFilter;

		// Filter string for the component list search input.
		FixedString<32> mComponentsFilter;

		// Currently selected entity in the inspector.
		EntityID mSelectedEntity = ecs::skNullEntity;

		TimedTooltip mActionTooltip;

		EntityCreator mEntityCreator;
	};

} // namespace lum::editor