#pragma once

#include "core/editor_common.hpp"
#include "core/utils/timed_tooltip.hpp"
#include "core/utils/fixed_string.hpp"

namespace lum::editor {

	class EntityEditor {
	public:

		void Handle( FScene* scene, EntityID entityID );
		void Toggle( bool value ) noexcept { bOpened = value; }

	private:

		void handle_left_panel( EntityID entityID );
		void handle_right_panel( EntityID entityID );
		void handle_footer( FScene* scene, EntityID entityID );
		void handle_apply( FScene* scene, EntityID entityID );
		void handle_closing( );

		static constexpr float32 mFooterHeight = 36.0f;
		std::vector<std::pair<const EditorComponentEntry*, bool>> mEntityComponents;
		TimedTooltip mActionTooltip;
		FixedString<64> mComponentsFilter;
		bool bOpened = false;
		bool bFindComponents = true;

		static constexpr inline bool skNewComponent = true;
		static constexpr inline bool skOldComponent = false;

	};

}