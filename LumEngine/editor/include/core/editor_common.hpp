#pragma once
#include "engine.hpp"
#include "core/editor_pch.hpp"
#include "core/core_common.hpp"
#include "core/utils/fonts.hpp"

namespace lum::editor {

	namespace detail {
		using EditorFn = void(*)(ecs::MEntityManager&, EntityID);
	}
	struct EditorComponentEntry {
		detail::EditorFn mEditorFn = {};
		detail::EditorFn mCreateFn = {};
		StringView mDisplayName = {};
		StringView mCategoryName = {};
		ComponentTypeID mTypeID = 0;
	};

	const EditorComponentEntry* FindComponentEntry( StringView name );

	struct FCollapsingHeaderArgs {
		HashedStr mID = {};
		StringView mLabel = {};
		StringView mIconOpened = ICON_FA_ANGLE_DOWN;
		StringView mIconClosed = ICON_FA_ANGLE_RIGHT;
		ImVec4 mColor = ImVec4( 0.8f, 0.8f, 0.8f, 1.0f );
		ImVec4 mColorHovered = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );
		ImFont* mFont = nullptr;
	};

	void CollapsingHeaderCustom( const FCollapsingHeaderArgs& args, bool& opened );

	void DrawRowBackground( const ImVec4& color );

	bool SearchField( ccharptr id, ccharptr hint, charptr buffer, usize bufferSize );

	void TooltipOnHover( const char* text );

}