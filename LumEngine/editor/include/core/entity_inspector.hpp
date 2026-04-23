#pragma once

#include "core/editor_common.hpp"
#include "core/entity_editor.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum::editor {

	class EntityInspector {
	public:

		void Handle( EntityID selectedEntity, FScene* scene );

	private:

		EntityEditor mEntityEditor;

		FixedString<64> mFieldsFilter;

	};

}