//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI dialog for inspecting and editing components of an existing entity.
//
//=============================================================================//
#pragma once

#include "core/editor_common.hpp"
#include "core/utils/timed_tooltip.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum::editor {

	// UI dialog for inspecting and editing components of an existing entity.
	class EntityEditor {
	public:

		// Updates and renders the entity editor dialog for the given entity.
		void Handle( FScene* scene, EntityID entityID );

		// Opens or closes the entity editor dialog.
		void Toggle( bool value ) noexcept { bOpened = value; }

	private:

		// Draws the left panel with available component list.
		void handle_left_panel( EntityID entityID );

		// Draws the right panel with the entity's current components.
		void handle_right_panel( EntityID entityID );

		// Draws the bottom footer with the apply and cancel buttons.
		void handle_footer( FScene* scene, EntityID entityID );

		// Applies pending component additions and removals to the entity.
		void handle_apply( FScene* scene, EntityID entityID );

		// Handles dialog close and resets transient state.
		void handle_closing( );

		static constexpr float32 mFooterHeight = 36.0f;

		// Current component list for the entity; bool indicates if the entry is newly added.
		std::vector<std::pair<const EditorComponentEntry*, bool>> mEntityComponents;
		std::vector<const EditorComponentEntry*> mComponentsToDelete;

		TimedTooltip mActionTooltip;

		// Filter string for the component search input.
		FixedString<64> mComponentsFilter;

		bool bOpened = false;

		// Controls whether the component list should be (re)fetched from the entity.
		bool bFindComponents = true;

		static constexpr inline bool skNewComponent = true;
		static constexpr inline bool skOldComponent = false;
	};

} // namespace lum::editor