//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Editor viewport handling rendering + camera control.
//
//=============================================================================//

#include "Core/Viewport.hpp"
#include "Core/EditorCamera.hpp"

namespace lum::editor {

	void Viewport::Update( float64 delta, rhi::TextureHandle frame, rhi::RenderDevice* dev, Window* window, render::Renderer* renderer ) {

		ImGui::Begin( "Viewport" );

		ImGuiDockNode* node = ImGui::GetWindowDockNode( );
		if (node) {
			node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
		}

		ImVec2 available = ImGui::GetContentRegionAvail( );
		mCamera.SetAspectRatio( ToFloat32( available.x ) / ToFloat32( available.y ) );
		renderer->UpdateCamera( mCamera.Update( delta, mCameraFocused ) );
		uint32 texID = dev->GetNativeHandle( frame );

		ImGui::Image( ( ImTextureID ) texID, available, ImVec2( 0, 1 ), ImVec2( 1, 0 ) );

		bool mousePressed = input::KeyPressed( input::Key::MOUSE_RIGHT );

		if (ImGui::IsWindowHovered( ) && mousePressed)
			mCameraFocused = true; 
		if (!mousePressed)
			mCameraFocused = false;
		
		if (!mCameraFocused)
			window->ToggleState( WindowStateFlags::Cursor, true );
		else {
			window->ToggleState( WindowStateFlags::Cursor, false );
			ImGui::GetIO( ).WantCaptureMouse = false;
			ImGui::GetIO( ).MousePos = ImVec2( -FLT_MAX, -FLT_MAX );
		}

		ImGui::End( ); // Viewport

	}


} // namespace lum::editor