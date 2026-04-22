#pragma once

#include "core/editor_common.hpp"
#include "core/utils/timed_tooltip.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum::editor {

	class EntityCreator {
	public:

		EntityCreator( ) { }

		void Handle( FScene* scene );
		void Toggle( bool value ) noexcept { bOpened = value; }

	private:

		void handle_left_panel( );
		void handle_right_panel( );
		void handle_footer( FScene* scene );
		void handle_creation( FScene* scene );
		void handle_closing( );

		std::vector<const EditorComponentEntry*> mAddedComponents;
		static constexpr float32 mFooterHeight = 36.0f;
		TimedTooltip mActionTooltip;
		FixedString<64> mComponentsFilter;
		FixedString<64> mEntityName;
		const EditorComponentEntry* mNameEntry = nullptr;
		bool bOpened = false;
		
	};

}