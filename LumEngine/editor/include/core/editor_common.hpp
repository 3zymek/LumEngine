#pragma once
#include "engine.hpp"
#include "core/editor_pch.hpp"
#include "core/core_common.hpp"
#include "core/utils/fonts.hpp"
#include "core/utils/style.hpp"

namespace lum::editor {

	//---------------------------------------------------------
	// Component registry
	//---------------------------------------------------------

	namespace detail {
		using EditorFn = void(*)(ecs::EntityManager&, EntityID);
	}

	/* @brief Metadata for a registered component type in the editor.
	*  Contains display, create, and delete callbacks along with UI info.
	*/
	struct EditorComponentMetadata {
		detail::EditorFn mDisplayFn = {};
		detail::EditorFn mCreateFn = {};
		detail::EditorFn mDeleteFn = {};
		StringView       mDisplayName = {};
		StringView       mCategoryName = {};
		ComponentTypeID  mTypeID = 0;
	};

	/* @brief Finds a registered component entry by display name. */
	const EditorComponentMetadata* FindComponentEntry( StringView name );

	//---------------------------------------------------------
	// Collapsing header / tree node
	//---------------------------------------------------------

	/* @brief Arguments for custom collapsing header and tree node widgets. */
	struct CollapsingHeaderArgs {
		HashedString	mID = {};
		StringView		mLabel = {};
		StringView		mIconOpened = ICON_FA_ANGLE_DOWN;
		StringView		mIconClosed = ICON_FA_ANGLE_RIGHT;
		ImVec4			mColor = ImVec4( 0.8f, 0.8f, 0.8f, 1.0f );
		ImVec4			mColorHovered = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );
		ImFont*			mFont = nullptr;
		float32			mMaxLabelWidth = 0.0f;
		bool			mSelected = false;
	};

	/* @brief Draws a custom collapsing header with icon and label.
	*  @param args  Display configuration.
	*  @param opened Output — true if the header is expanded.
	*/
	void CollapsingHeaderCustom( const CollapsingHeaderArgs& args, bool& opened );

	/* @brief Draws a custom tree node with chevron icon, hover/select highlight, and indent line.
	*  @param args     Display configuration.
	*  @param opened   Output — true if the node is expanded.
	*  @param innerFn  Lambda called when the node is open; renders children.
	*  @return True if the node label was clicked (not the arrow).
	*/
	bool TreeNodeCustom( const CollapsingHeaderArgs& args, bool& opened, auto innerFn ) {

		ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0, 0, 0, 0 ) );
		ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0, 0, 0, 0 ) );
		ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0, 0, 0, 0 ) );
		ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0, 0, 0, 0 ) );

		ImGui::PushStyleVar( ImGuiStyleVar_IndentSpacing, 12.0f );

		ImGui::PushID( args.mID );

		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_DefaultOpen;

		if (args.mSelected)
			flags |= ImGuiTreeNodeFlags_Selected;

		opened = ImGui::TreeNodeEx( "##hidden", flags );
		ImGui::PopID( );
		ImGui::PopStyleColor( 4 );

		// compute item rect for custom hit-testing and drawing
		ImVec2 itemMin = ImGui::GetItemRectMin( );
		ImVec2 itemMax = ImGui::GetItemRectMax( );
		itemMax.x = ImGui::GetWindowPos( ).x + ImGui::GetWindowSize( ).x - ImGui::GetStyle( ).ScrollbarSize;

		// invisible button spanning the full row for click detection
		ImGui::SetCursorScreenPos( itemMin );
		ImGui::InvisibleButton( "##treeclick", ImVec2( itemMax.x - itemMin.x, itemMax.y - itemMin.y ) );

		bool bClicked = ImGui::IsItemClicked( );
		bool bHovered = ImGui::IsItemHovered( );

		// draw background highlight
		if (bHovered || args.mSelected) {
			ImVec4 bgColor = args.mSelected ? style::skItemSelected : style::skItemHovered;
			ImGui::GetWindowDrawList( )->AddRectFilled( itemMin, itemMax, ImGui::GetColorU32( bgColor ) );
		}

		// draw chevron and label
		ImVec2   textPos = ImVec2( itemMin.x + 5.0f, itemMin.y );
		ccharptr chevron = opened ? args.mIconOpened.data( ) : args.mIconClosed.data( );
		float32  chevronWidth = ImGui::CalcTextSize( args.mIconOpened.data( ) ).x;
		chevronWidth = std::max( chevronWidth, ImGui::CalcTextSize( args.mIconClosed.data( ) ).x );
		ImU32    color = ImGui::GetColorU32( args.mColor );

		if (args.mFont) ImGui::PushFont( args.mFont );
		ImGui::GetWindowDrawList( )->AddText( textPos, color, chevron );
		ImGui::GetWindowDrawList( )->AddText(
			ImVec2( textPos.x + chevronWidth + 6.0f, textPos.y ),
			color,
			args.mLabel.data( ),
			args.mMaxLabelWidth > 0.0f ? args.mLabel.data( ) + args.mLabel.size( ) : nullptr
		);
		if (args.mFont) ImGui::PopFont( );

		// draw children and indent line
		if (opened) {
			ImVec2 lineStart = ImGui::GetCursorScreenPos( );
			lineStart.x = itemMin.x + 6.0f;

			innerFn( );

			ImVec2 lineEnd = ImGui::GetCursorScreenPos( );
			lineEnd.x = lineStart.x;

			ImGui::GetWindowDrawList( )->AddLine(
				lineStart, lineEnd,
				IM_COL32( 80, 80, 80, 255 ), 1.0f
			);

			ImGui::TreePop( );
		}

		ImGui::PopStyleVar( );

		return bClicked;
	}

	//---------------------------------------------------------
	// Misc widgets
	//---------------------------------------------------------

	/* @brief Draws a colored background behind the current row. */
	void DrawRowBackground( const ImVec4& color );

	/* @brief Draws a search input field with a placeholder hint.
	*  @return True if the content changed this frame.
	*/
	bool SearchField( ccharptr id, ccharptr hint, charptr buffer, usize bufferSize );

	/* @brief Shows a tooltip when the last item is hovered. */
	void TooltipOnHover( const char* text );

	//---------------------------------------------------------
	// Popup context menu
	//---------------------------------------------------------

	/* @brief Single item in a popup context menu. */
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

	/* @brief Arguments for a popup context menu. */
	struct FPopupContextArgs {
		std::vector<PopupContextItem> mItems;
	};

	/* @brief Draws a right-click popup context menu with icon, label, and shortcut columns. */
	void DrawPopupContext( const FPopupContextArgs& args );

} // namespace lum::editor