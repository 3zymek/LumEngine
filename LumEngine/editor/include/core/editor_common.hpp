#pragma once

#include "core/editor_pch.hpp"
#include "core/core_common.hpp"
#include "core/utils/fonts.hpp"

namespace lum::editor {

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