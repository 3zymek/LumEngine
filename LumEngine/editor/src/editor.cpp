#include "editor.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace lum {

	void Editor::Run( ) {

		ImGui::GetIO( ).ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		while (mEngine.IsRunning( )) {

			float64 delta = mEngine.GetDeltaTime( );

			mEngine.BeginFrame( );
			begin_imgui( );

			mEngine.Tick( );

			DrawViewport( delta );

			mEngine.EndFrame( );
			end_imgui( );
			mEngine.GetPlatform( ).mRenderDevice->SwapBuffers( );


		}

	}
	void Editor::DrawSceneHierarchy( ) {

	}
	void Editor::DrawInspector( ) {

	}
	void Editor::DrawViewport( float64 delta ) {

		ImGuiDockNodeFlags dockFlags =
			ImGuiDockNodeFlags_NoUndocking |
			ImGuiDockNodeFlags_NoResize;

		ImGuiID dockID = ImGui::DockSpaceOverViewport( dockFlags );
		
		if (!bLayoutInitialized) {
			bLayoutInitialized = true;

			ImGui::DockBuilderRemoveNode( dockID );
			ImGui::DockBuilderAddNode( dockID, ImGuiDockNodeFlags_DockSpace );
			ImGui::DockBuilderSetNodeSize( dockID, ImGui::GetMainViewport( )->Size );

			ImGuiID left, right;
			ImGui::DockBuilderSplitNode( dockID, ImGuiDir_Left, 0.25f, &left, &right );

			ImGuiID middle, middleRight;
			ImGui::DockBuilderSplitNode( right, ImGuiDir_Left, 0.75f, &middle, &middleRight );

			ImGuiID console, viewport;
			ImGui::DockBuilderSplitNode( middle, ImGuiDir_Down, 0.25f, &console, &viewport );

			ImGui::DockBuilderDockWindow( "Console", console );
			ImGui::DockBuilderDockWindow( "Viewport", viewport );
			ImGui::DockBuilderDockWindow( "ECS Inspector", middleRight );
			ImGui::DockBuilderDockWindow( "File Explorer", left );

			ImGui::DockBuilderFinish( dockID );
		}

		ImGui::Begin( "File Explorer" );

		ImGui::Text( "Here will be file explorer" );

		ImGui::End( ); // File Ex plorer

		ImGui::Begin( "Viewport" );

		ImVec2 available = ImGui::GetContentRegionAvail( );

		mCamera.SetAspectRatio( ( float32 ) available.x / ( float32 ) available.y );
		mEngine.GetRender( ).mRenderer.UpdateCamera( mCamera.Update( delta ) );

		uint32 texID = mEngine.GetPlatform( ).mRenderDevice->GetNativeHandle(
			mEngine.GetRender( ).mRenderer.GetFrameTexture( )
		);

		ImGui::Image( ( ImTextureID ) texID, available, ImVec2( 0, 1 ), ImVec2( 1, 0 ) );



		ImGui::End( ); // Viewport

		ImGui::Begin( "ECS Inspector" );
		ImGui::End( ); // ECS Inspector

		ImGui::Begin( "Console" );

		ImGui::Text( "Here will be console" );

		ImGui::End( ); // Console
	}



	void Editor::init_imgui( Window* window ) {

		ImGui::CreateContext( );
		ImGui_ImplGlfw_InitForOpenGL( static_cast< GLFWwindow* >(window->GetNativeWindow( )), true );
		ImGui_ImplOpenGL3_Init( "#version 450" );

		ImGuiStyle& style = ImGui::GetStyle( );
		ImVec4* colors = style.Colors;

		// Zaokrąglenia
		style.WindowRounding = 6.0f;
		style.FrameRounding = 4.0f;
		style.TabRounding = 4.0f;
		style.ScrollbarRounding = 4.0f;
		style.GrabRounding = 4.0f;
		style.PopupRounding = 4.0f;

		// Rozmiary
		style.WindowPadding = ImVec2( 10, 10 );
		style.FramePadding = ImVec2( 6, 4 );
		style.ItemSpacing = ImVec2( 8, 5 );
		style.ScrollbarSize = 10.0f;
		style.GrabMinSize = 10.0f;
		style.WindowBorderSize = 0.0f;
		style.FrameBorderSize = 0.0f;
		style.TabBorderSize = 0.0f;

		// Ciemny motyw — Unreal-style
		colors[ ImGuiCol_WindowBg ] = ImVec4( 0.10f, 0.10f, 0.10f, 1.00f );
		colors[ ImGuiCol_ChildBg ] = ImVec4( 0.08f, 0.08f, 0.08f, 1.00f );
		colors[ ImGuiCol_PopupBg ] = ImVec4( 0.12f, 0.12f, 0.12f, 1.00f );
		colors[ ImGuiCol_Border ] = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );

		colors[ ImGuiCol_TitleBg ] = ImVec4( 0.06f, 0.06f, 0.06f, 1.00f );
		colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.06f, 0.06f, 0.06f, 1.00f );
		colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.06f, 0.06f, 0.06f, 1.00f );

		colors[ ImGuiCol_FrameBg ] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
		colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
		colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.22f, 0.22f, 0.22f, 1.00f );

		colors[ ImGuiCol_Button ] = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
		colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.26f, 0.26f, 0.26f, 1.00f );
		colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.32f, 0.32f, 0.32f, 1.00f );

		colors[ ImGuiCol_Header ] = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
		colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.24f, 0.24f, 0.24f, 1.00f );
		colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f );

		colors[ ImGuiCol_Tab ] = ImVec4( 0.08f, 0.08f, 0.08f, 1.00f );
		colors[ ImGuiCol_TabHovered ] = ImVec4( 0.22f, 0.22f, 0.22f, 1.00f );
		colors[ ImGuiCol_TabActive ] = ImVec4( 0.16f, 0.16f, 0.16f, 1.00f );
		colors[ ImGuiCol_TabUnfocused ] = ImVec4( 0.08f, 0.08f, 0.08f, 1.00f );
		colors[ ImGuiCol_TabUnfocusedActive ] = ImVec4( 0.12f, 0.12f, 0.12f, 1.00f );

		colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.08f, 0.08f, 0.08f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.24f, 0.24f, 0.24f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.30f, 0.30f, 0.30f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.36f, 0.36f, 0.36f, 1.00f );

		colors[ ImGuiCol_CheckMark ] = ImVec4( 0.80f, 0.80f, 0.80f, 1.00f );
		colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.40f, 0.40f, 0.40f, 1.00f );
		colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.60f, 0.60f, 0.60f, 1.00f );

		colors[ ImGuiCol_Separator ] = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
		colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.35f, 0.35f, 0.35f, 1.00f );
		colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );

		colors[ ImGuiCol_DockingPreview ] = ImVec4( 0.40f, 0.40f, 0.40f, 0.60f );
		colors[ ImGuiCol_DockingEmptyBg ] = ImVec4( 0.08f, 0.08f, 0.08f, 1.00f );

		colors[ ImGuiCol_Text ] = ImVec4( 0.90f, 0.90f, 0.90f, 1.00f );
		colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.40f, 0.40f, 0.40f, 1.00f );

		ImGui::GetIO( ).Fonts->AddFontFromFileTTF( "C:/Windows/Fonts/segoeui.ttf", 15.0f );

	}
	void Editor::begin_imgui( ) {

		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplGlfw_NewFrame( );
		ImGui::NewFrame( );

		ImGui::GetIO( ).WantCaptureKeyboard = false;

	}
	void Editor::end_imgui( ) {

		ImGui::Render( );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

	}

}