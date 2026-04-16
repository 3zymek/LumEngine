#pragma once
#include "core/editor_common.hpp"

namespace lum::editor::style {

	// ── Palette ─────────────────────────────────────────────────────────────
	inline const ImVec4 skAccent = ImVec4( 0.42f, 0.39f, 1.00f, 1.00f );
	inline const ImVec4 skAccentHovered = ImVec4( 0.54f, 0.51f, 1.00f, 1.00f );
	inline const ImVec4 skAccentActive = ImVec4( 0.31f, 0.29f, 0.67f, 1.00f );

	inline const ImVec4 skBg = ImVec4( 0.09f, 0.09f, 0.09f, 1.00f );
	inline const ImVec4 skBgChild = ImVec4( 0.11f, 0.11f, 0.11f, 1.00f );
	inline const ImVec4 skBgPopup = ImVec4( 0.13f, 0.13f, 0.13f, 1.00f );

	inline const ImVec4 skTitleBg = ImVec4( 0.07f, 0.07f, 0.07f, 1.00f );
	inline const ImVec4 skTitleBgActive = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );

	inline const ImVec4 skBorder = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );

	inline const ImVec4 skFrameBg = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
	inline const ImVec4 skFrameBgHovered = ImVec4( 0.22f, 0.22f, 0.22f, 1.00f );
	inline const ImVec4 skFrameBgActive = ImVec4( 0.27f, 0.27f, 0.27f, 1.00f );

	inline const ImVec4 skHeader = ImVec4( 0.16f, 0.16f, 0.16f, 1.00f );

	inline const ImVec4 skTab = ImVec4( 0.11f, 0.11f, 0.11f, 1.00f );
	inline const ImVec4 skTabHovered = ImVec4( 0.22f, 0.22f, 0.22f, 1.00f );
	inline const ImVec4 skTabActive = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
	inline const ImVec4 skTabUnfocused = ImVec4( 0.09f, 0.09f, 0.09f, 1.00f );
	inline const ImVec4 skTabUnfocusedActive = ImVec4( 0.13f, 0.13f, 0.13f, 1.00f );

	inline const ImVec4 skItemHovered = ImVec4( 0.12f, 0.11f, 0.18f, 1.00f );
	inline const ImVec4 skItemSelected = ImVec4( 0.17f, 0.15f, 0.25f, 1.00f );
	inline const ImVec4 skWindowBackground = skBg;

	// ── Theme ────────────────────────────────────────────────────────────────
	inline void ApplyTheme( ) {
		ImGuiStyle& style = ImGui::GetStyle( );
		ImVec4* colors = style.Colors;

		// Backgrounds
		colors[ ImGuiCol_WindowBg ]	= skBg;
		colors[ ImGuiCol_ChildBg ]	= skBgChild;
		colors[ ImGuiCol_PopupBg ]	= skBgPopup;

		// Title
		colors[ ImGuiCol_TitleBg ]			= skTitleBg;
		colors[ ImGuiCol_TitleBgActive ]	= skTitleBgActive;
		colors[ ImGuiCol_TitleBgCollapsed ] = skTitleBg;

		// Border
		colors[ ImGuiCol_Border ]		= skBorder;
		colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );

		// Frame
		colors[ ImGuiCol_FrameBg ]			= skFrameBg;
		colors[ ImGuiCol_FrameBgHovered ]	= skFrameBgHovered;
		colors[ ImGuiCol_FrameBgActive ]	= skFrameBgActive;

		// Widgets
		colors[ ImGuiCol_CheckMark ]		= skAccent;
		colors[ ImGuiCol_SliderGrab ]		= skAccent;
		colors[ ImGuiCol_SliderGrabActive ] = skAccentHovered;

		// Button
		colors[ ImGuiCol_Button ]			= skHeader;
		colors[ ImGuiCol_ButtonHovered ]	= skAccent;
		colors[ ImGuiCol_ButtonActive ]		= skAccentActive;

		// Header (TreeNode, Selectable, etc.)
		colors[ ImGuiCol_Header ]			= skHeader;
		colors[ ImGuiCol_HeaderHovered ]	= skItemHovered;
		colors[ ImGuiCol_HeaderActive ]		= skAccent;

		// Separator
		colors[ ImGuiCol_Separator ]		= skBorder;
		colors[ ImGuiCol_SeparatorHovered ] = skAccent;
		colors[ ImGuiCol_SeparatorActive ]	= skAccentHovered;

		// Tabs
		colors[ ImGuiCol_Tab ]							= skTab;
		colors[ ImGuiCol_TabHovered ]					= skTabHovered;
		colors[ ImGuiCol_TabActive ]					= skTabActive;
		colors[ ImGuiCol_TabUnfocused ]					= skTabUnfocused;
		colors[ ImGuiCol_TabUnfocusedActive ]			= skTabUnfocusedActive;
		colors[ ImGuiCol_TabSelectedOverline ]			= skAccent;
		colors[ ImGuiCol_TabDimmedSelectedOverline ]	= skAccentActive;

		// Scrollbar
		colors[ ImGuiCol_ScrollbarBg ]			= skBg;
		colors[ ImGuiCol_ScrollbarGrab ]		= ImVec4( 0.22f, 0.22f, 0.22f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrabActive ]	= skAccent;

		// Style
		style.WindowRounding	= 4.0f;
		style.ChildRounding		= 4.0f;
		style.FrameRounding		= 3.0f;
		style.PopupRounding		= 4.0f;
		style.TabRounding		= 3.0f;
		style.ScrollbarRounding = 3.0f;
		style.GrabRounding		= 3.0f;

		style.WindowPadding		= ImVec2( 10.0f, 10.0f );
		style.FramePadding		= ImVec2( 6.0f, 4.0f );
		style.ItemSpacing		= ImVec2( 6.0f, 4.0f );
		style.ItemInnerSpacing	= ImVec2( 4.0f, 4.0f );
		style.IndentSpacing		= 18.0f;
		style.ScrollbarSize		= 12.0f;
		style.GrabMinSize		= 8.0f;

		style.WindowBorderSize	= 1.0f;
		style.FrameBorderSize	= 0.0f;
		style.PopupBorderSize	= 1.0f;
	}

}