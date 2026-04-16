#pragma once

#include "core/editor_common.hpp"
#include "entity/ecs_common.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum { class FScene; }

namespace lum::editor {

	class SceneInspector {
	public:

		void Update( FScene* scene );
		EntityID GetSelectedEntity( ) const noexcept { return mSelectedEntity; }

	private:
		
		FixedString<64> mEntitiesFilter;
		EntityID mSelectedEntity = ecs::skNullEntity;

	};

}