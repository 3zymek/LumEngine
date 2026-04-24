//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI panel for inspecting and editing components of a selected entity.
//
//=============================================================================//
#pragma once

#include "core/editor_common.hpp"
#include "core/entity_editor.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum::editor {

	// UI panel for inspecting and editing components of a selected entity.
	class EntityInspector {
	public:

		// Updates and renders the inspector for the given entity.
		void Handle( EntityID selectedEntity, FScene* scene );

	private:

		EntityEditor mEntityEditor;

		// Filter string for the component fields search input.
		FixedString<64> mFieldsFilter;
	};

} // namespace lum::editor