//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI panel for browsing, selecting and managing scene entities.
//
//=============================================================================//
#pragma once

#include "Core/EditorCommon.hpp"
#include "Core/Utils/FixedString.hpp"
#include "Core/Utils/TimedTooltip.hpp"
#include "Core/EntityCreator.hpp"

namespace lum::editor {

	// UI panel for browsing, selecting and managing scene entities.
	class SceneInspector {
	public:

		void Initialize( ev::EventBus* eventBus );
			
		// Updates and renders the scene inspector panel.
		void Update( Scene* scene );

		// Returns the currently selected entity, or kNullEntity if none.
		EntityID GetSelectedEntity( ) const noexcept { return mSelectedEntity; }

	private:

		void draw_entity( Scene* scene, EntityID entity );

		// Filter string for the entity list search input.
		FixedString<64> mEntitiesFilter;

		// Filter string for the component list search input.
		FixedString<32> mComponentsFilter;

		// Currently selected entity in the inspector.
		EntityID mSelectedEntity = ecs::kNullEntity;

		TimedTooltip mActionTooltip;

		EntityCreator mEntityCreator;

	};

} // namespace lum::editor