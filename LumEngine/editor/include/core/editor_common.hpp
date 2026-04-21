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
	};

	struct FCollapsingHeaderArgs {
		HashedStr mID = {};
		StringView mLabel = {};
		StringView mIconOpened = ICON_FA_ANGLE_DOWN;
		StringView mIconClosed = ICON_FA_ANGLE_RIGHT;
		ImVec4 mColor = ImVec4( 0.8f, 0.8f, 0.8f, 1.0f );
		ImVec4 mColorHovered = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );
		ImFont* mFont = nullptr;
	};

	inline void CollapsingHeaderCustom( const FCollapsingHeaderArgs& args, bool& opened ) {
		
		ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0, 0, 0, 0 ) );
		ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0, 0, 0, 0 ) );
		ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0, 0, 0, 0 ) );

		ImGui::SetNextItemOpen( true, ImGuiCond_Once );
		ImGui::PushID( args.mID );
		opened = ImGui::CollapsingHeader( "##hidden" );
		ImGui::PopID( );
		ImGui::PopStyleColor( 4 );

		bool hovered = ImGui::IsItemHovered( );
		ImVec2 min = ImGui::GetItemRectMin( );
		ImVec2 textPos = ImVec2( min.x + 5.0f, min.y );

		ccharptr chevron = opened ? args.mIconOpened.data( ) : args.mIconClosed.data( );

		float32 chevronWidth = ImGui::CalcTextSize( args.mIconOpened.data( ) ).x;
		chevronWidth = std::max( chevronWidth, ImGui::CalcTextSize( args.mIconClosed.data( ) ).x );

		ImU32 color = hovered ? ImGui::GetColorU32( args.mColorHovered ) : ImGui::GetColorU32( args.mColor );

		if(args.mFont) ImGui::PushFont( args.mFont );
		ImGui::GetWindowDrawList( )->AddText( textPos, color, chevron );
		ImGui::GetWindowDrawList( )->AddText(
			ImVec2( textPos.x + chevronWidth + 6.0f, textPos.y ),
			color,
			args.mLabel.data( )
		);
		if(args.mFont) ImGui::PopFont( );

	}

	inline void DrawRowBackground( const ImVec4& color ) {

		ImVec2 pos = ImGui::GetCursorScreenPos( );
		ImGui::SetCursorScreenPos( ImVec2( pos.x + 8.0f, pos.y ) );
		float32 width = ImGui::GetContentRegionAvail( ).x;
		float32 height = ImGui::GetTextLineHeightWithSpacing( );
		ImGui::GetWindowDrawList( )->AddRectFilled(
			pos,
			ImVec2( pos.x + width, pos.y + height ),
			ImGui::ColorConvertFloat4ToU32( color ),
			4.0f
		);
	}

	inline bool SearchField( ccharptr id, ccharptr hint, charptr buffer, usize bufferSize ) {
		ImGui::PushID( id );

		bool changed = ImGui::InputTextWithHint( "##input", hint, buffer, bufferSize );

		ImVec2 min = ImGui::GetItemRectMin( ); // Get last element left-upper corner
		ImVec2 max = ImGui::GetItemRectMax( ); // Get last element right-lower corner

		float32 height = ImGui::GetFrameHeight( ); // Get input text height

		ImGui::SameLine( 0, -height );

		ImGui::SetCursorScreenPos(
			ImVec2( max.x - height, min.y )
		);

		ImGui::TextUnformatted( ICON_FA_SEARCH );

		ImGui::PopID( );

		return changed;
	}

	inline void TooltipOnHover( const char* text ) {
		if (ImGui::IsItemHovered( ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay )) {
			ImGui::SetTooltip( text );
		}
	}

}