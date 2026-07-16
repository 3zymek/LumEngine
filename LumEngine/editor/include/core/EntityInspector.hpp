//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI panel for inspecting and editing components of a selected entity.
//
//=============================================================================//
#pragma once

#include "Core/EditorCommon.hpp"
#include "Core/EntityEditor.hpp"
#include "Core/Utils/FixedString.hpp"

namespace lum::editor {

	// UI panel for inspecting and editing components of a selected entity.
	class EntityInspector {
	public:

		// Updates and renders the inspector for the given entity.
		void Update( EntityID selectedEntity, Scene* scene );

	private:

		EntityEditor mEntityEditor;

		// Filter string for the component fields search input.
		FixedString<64> mFieldsFilter;
	};

} // namespace lum::editor