#pragma once

#include "core/editor_common.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum::editor {

	class ComponentsCreator {
	public:

		void Handle( FScene* scene, EntityID entity );
		void Toggle( bool value ) noexcept { bOpened = value; }

	private:

		static constexpr float32 mFooterHeight = 32.0f;
		FixedString<32> mComponentsFilter;
		bool bOpened = false;

	};

}