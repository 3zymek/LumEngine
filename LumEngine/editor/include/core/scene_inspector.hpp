#pragma once

#include "core/editor_common.hpp"
#include "entity/ecs_common.hpp"

namespace lum { class FScene; }

namespace lum::editor {

	class SceneInspector {
	public:

		void Update( FScene* scene );
		EntityID GetSelectedEntity( ) const noexcept { return mSelectedEntity; }

	private:

		EntityID mSelectedEntity;

	};

}