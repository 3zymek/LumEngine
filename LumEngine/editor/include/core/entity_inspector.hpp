#pragma once

#include "core/editor_common.hpp"
#include "core/component_creator.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum::editor {

	class EntityInspector {
	public:

		void Handle( EntityID selectedEntity, FScene* scene );

	private:

		ComponentsCreator mComponentsCreator;

		FixedString<64> mFieldsFilter;

	};

}