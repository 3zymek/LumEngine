//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI dialog for creating new entities with a custom component set.
//
//=============================================================================//
#pragma once

#include "Core/EditorCommon.hpp"
#include "Core/Utils/TimedTooltip.hpp"
#include "Core/Utils/FixedString.hpp"

namespace lum::editor {

	// UI dialog for creating new entities with a custom component set.
	class EntityCreator {
	public:

		void Initialize( ev::EventBus* eventBus );

		// Updates and renders the entity creator dialog.
		void Handle( Scene* scene );

		// Opens or closes the entity creator dialog.
		void Toggle( bool value ) noexcept { bOpened = value; }

	private:

		// Draws the left panel with available component list.
		void handle_left_panel( );

		// Draws the right panel with added components preview.
		void handle_right_panel( );

		// Draws the bottom footer with entity name input and confirm button.
		void handle_footer( Scene* scene );

		// Spawns the entity with selected components into the scene.
		void handle_creation( Scene* scene );

		// Handles dialog close and resets transient state.
		void handle_closing( );


		// Components selected to be added to the new entity.
		std::vector<const EditorComponentMetadata*> mAddedComponents;

		static constexpr float32 mFooterHeight = 36.0f;
		TimedTooltip mActionTooltip;

		// Filter string for the component search input.
		FixedString<64> mComponentsFilter;

		// Name to assign to the newly created entity.
		FixedString<64> mEntityName;

		// Component entry used to drive the entity name field, if any.
		const EditorComponentMetadata* mNameEntry = nullptr;

		const EditorComponentMetadata* mSelectedComponent = nullptr;

		bool bOpened = false;

	};

} // namespace lum::editor