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
		detail::EditorFn mDisplayFn = {};
		detail::EditorFn mCreateFn = {};
		detail::EditorFn mDeleteFn = {};
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

	struct PopupContextItem {

		template<typename tLambda>
		PopupContextItem(
			StringView icon,
			StringView label,
			StringView shortcut,
			tLambda&& lambda
		) :
			mIcon( icon ),
			mLabel( label ),
			mShortcut( shortcut ) {
			LUM_SASSERT( sizeof( tLambda ) <= sizeof( mStorage ) && "Lambda's too big" );
			new (&mStorage) tLambda( std::forward<tLambda>( lambda ) );
			mInvoke = []( vptr buf ) {
				(*reinterpret_cast< tLambda* >(buf))();
				};
		}

		void Invoke( ) const { if (mInvoke) mInvoke( ( vptr ) &mStorage ); }

		StringView mIcon = {};
		StringView mLabel = {};
		StringView mShortcut = {};
		std::aligned_storage_t<256, alignof(std::max_align_t)> mStorage;
		void(*mInvoke)(vptr) = nullptr;

	};
	struct FPopupContextArgs {
		std::vector<PopupContextItem> mItems;
	};
	inline void DrawPopupContext( const FPopupContextArgs& args ) {

		if (ImGui::BeginPopupContextItem( )) {
			char buffer[ 256 ]{};
			for (auto& item : args.mItems) {
				FormatString( buffer, "%s %s", item.mIcon.data( ), item.mLabel.data( ) );
				if (ImGui::MenuItem( buffer, item.mShortcut.data( ) )) {
					item.Invoke();
				}
			}
			ImGui::EndPopup( );
		}

	}

}