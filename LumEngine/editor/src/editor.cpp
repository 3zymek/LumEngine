#include "editor.hpp"
#include "entity/components/name.hpp"
#include "editor_dep_manager.generated.hpp"
#include "core/utils/fonts.hpp"
#include "core/utils/shortcuts.hpp"

namespace lum::editor {

	void Editor::Initialize( ) {

		mEngine.Initialize( "C:/Users/szymek/Desktop/lumen_assets" ); mEngine.SetScene( "scene.lsc" );

		mCamera.Initialize( mEngine.GetEventBus( ) );

		init_imgui( &mEngine.GetPlatform( ).mWindow );

		RegisterEditorComponents( skDrawFunctions );

	}

	void Editor::Finalize( ) {
		mEngine.Finalize( ); 
	}

	void Editor::Run( ) {

		ImGui::GetIO( ).ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		while (mEngine.IsRunning( )) {

			float64 delta = mEngine.GetDeltaTime( );

			mEngine.BeginFrame( );
			begin_imgui( );

			mEngine.Tick( );

			Update( delta );

			mEngine.EndFrame( );
			end_imgui( );
			mEngine.GetPlatform( ).mRenderDevice->SwapBuffers( );

		}

	}
	void Editor::Update( float64 delta ) {

		FScene* scene = mEngine.GeModuleScene( ).mSceneMgr.GetCurrentScene( );

		draw_layout( );
		draw_menu_bar( );
		draw_viewport( delta );
		mSceneInspector.Update( scene );
		draw_entity_inspector( );

		mExplorer.Update( AssetLoader::GetProjectRoot( ) );

		mConsole.Update( );

	}

	void Editor::draw_viewport( float64 delta ) {

		ImGui::Begin( "Viewport" );
		
		ImGuiDockNode* node = ImGui::GetWindowDockNode( );
		if (node) {
			node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
		}

		ImVec2 available = ImGui::GetContentRegionAvail( );

		mCamera.SetAspectRatio( ( float32 ) available.x / ( float32 ) available.y );
		mEngine.GetModuleRender( ).mRenderer.UpdateCamera( mCamera.Update( delta ) );

		uint32 texID = mEngine.GetPlatform( ).mRenderDevice->GetNativeHandle(
			mEngine.GetModuleRender( ).mRenderer.GetFrameTexture( )
		);

		ImGui::Image( ( ImTextureID ) texID, available, ImVec2( 0, 1 ), ImVec2( 1, 0 ) );

		ImGui::End( ); // Viewport

	}
	void Editor::draw_menu_bar( ) {

		if (ImGui::BeginMainMenuBar( )) {
			if (ImGui::BeginMenu( "Scene" )) {
				ImGui::MenuItem( "New Scene", "Ctrl + N" );
				ImGui::MenuItem( "Open scene", "Ctrl + O" );
				ImGui::Separator( );
				if (ImGui::MenuItem( "Quit", "Ctrl + Q" )) {
					mEngine.GetPlatform( ).mWindow.Close( );
				} ImGui::SameLine( ); ImGui::TextUnformatted( ICON_FA_TIMES );
				ImGui::EndMenu( );
			}
			if (ImGui::BeginMenu( "Project" )) {
				ImGui::EndMenu( );
			}
			ImGui::EndMainMenuBar( );
		}
	}
	void Editor::draw_entity_inspector( ) {

		auto* scene = mEngine.GeModuleScene( ).mSceneMgr.GetCurrentScene( );

		ImGui::Begin( "Entity Inspector" );
		if (mSceneInspector.GetSelectedEntity() != ecs::skNullEntity) {
			scene->mEntityMgr.ForEachComponent(
				mSceneInspector.GetSelectedEntity( ),
				[&]( ecs::detail::BasePool* pool ) {
					if (ImGui::CollapsingHeader( pool->GetName( ).data( ) )) {
						skDrawFunctions[ HashStr( pool->GetName( ) ) ]( scene->mEntityMgr, mSceneInspector.GetSelectedEntity( ) );
					}
				} );
		}
		ImGui::End( );

	}
	void Editor::draw_layout( ) {

		static bool bLayoutInitialized = false;

		ImGuiDockNodeFlags dockFlags =
			ImGuiDockNodeFlags_NoUndocking |
			ImGuiDockNodeFlags_NoResize;

		ImGuiID dockID = ImGui::DockSpaceOverViewport( dockFlags );

		if (!bLayoutInitialized) {

			bLayoutInitialized = true;

			ImGui::DockBuilderRemoveNode( dockID );
			ImGui::DockBuilderAddNode( dockID, ImGuiDockNodeFlags_DockSpace );
			ImGui::DockBuilderSetNodeSize( dockID, ImGui::GetMainViewport( )->Size );

			ImGuiID left, halfRight;
			ImGui::DockBuilderSplitNode( dockID, ImGuiDir_Left, 0.25f, &left, &halfRight );

			ImGuiID middle, right;
			ImGui::DockBuilderSplitNode( halfRight, ImGuiDir_Left, 0.75f, &middle, &right );

			ImGuiID console, viewport;
			ImGui::DockBuilderSplitNode( middle, ImGuiDir_Down, 0.25f, &console, &viewport );

			ImGuiID sceneInspector, entityInspector;
			ImGui::DockBuilderSplitNode( right, ImGuiDir_Up, 1.0f, &sceneInspector, &entityInspector );

			ImGui::DockBuilderDockWindow( "Console", console );
			ImGui::DockBuilderDockWindow( "Viewport", viewport );
			ImGui::DockBuilderDockWindow( "Scene Inspector", sceneInspector );
			ImGui::DockBuilderDockWindow( "Entity Inspector", entityInspector );
			ImGui::DockBuilderDockWindow( "File Explorer", left );

			ImGui::DockBuilderFinish( dockID );

			set_flags_recursive( ImGui::DockBuilderGetNode( dockID ), ImGuiDockNodeFlags_NoWindowMenuButton );
		}

	}
	void Editor::init_imgui( Window* window ) {

		ImGui::CreateContext( );
		ImGui_ImplGlfw_InitForOpenGL( static_cast< GLFWwindow* >(window->GetNativeWindow( )), true );
		ImGui_ImplOpenGL3_Init( "#version 450" );

		ImGuiStyle& style = ImGui::GetStyle( );
		ImVec4* colors = style.Colors;

		colors[ ImGuiCol_WindowBg ] = ImVec4( 0.09f, 0.09f, 0.09f, 1.00f );
		colors[ ImGuiCol_ChildBg ] = ImVec4( 0.11f, 0.11f, 0.11f, 1.00f );
		colors[ ImGuiCol_PopupBg ] = ImVec4( 0.13f, 0.13f, 0.13f, 1.00f );

		colors[ ImGuiCol_TitleBg ] = ImVec4( 0.07f, 0.07f, 0.07f, 1.00f );
		colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
		colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.07f, 0.07f, 0.07f, 1.00f );

		colors[ ImGuiCol_Border ] = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
		colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );

		colors[ ImGuiCol_FrameBg ] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
		colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.19f, 0.19f, 0.19f, 1.00f );
		colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.24f, 0.24f, 0.24f, 1.00f );

		colors[ ImGuiCol_CheckMark ] = ImVec4( 0.85f, 0.75f, 0.40f, 1.00f );
		colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.85f, 0.75f, 0.40f, 1.00f );
		colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.95f, 0.85f, 0.50f, 1.00f );

		colors[ ImGuiCol_Button ] = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
		colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.85f, 0.75f, 0.40f, 1.00f );
		colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.75f, 0.65f, 0.30f, 1.00f );

		colors[ ImGuiCol_Header ] = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );

		colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.85f, 0.75f, 0.40f, 1.00f );
		colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.33f, 0.55f, 0.86f, 1.00f );

		colors[ ImGuiCol_Separator ] = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
		colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.85f, 0.75f, 0.40f, 1.00f );
		colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.95f, 0.85f, 0.50f, 1.00f );

		// Tabs
		colors[ ImGuiCol_Tab ] = ImVec4( 0.11f, 0.11f, 0.11f, 1.00f );
		colors[ ImGuiCol_TabHovered ] = ImVec4( 0.26f, 0.26f, 0.26f, 1.00f );
		colors[ ImGuiCol_TabActive ] = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
		colors[ ImGuiCol_TabUnfocused ] = ImVec4( 0.09f, 0.09f, 0.09f, 1.00f );
		colors[ ImGuiCol_TabUnfocusedActive ] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );

		// Dockspace
		colors[ ImGuiCol_DockingPreview ] = ImVec4( 0.85f, 0.75f, 0.40f, 0.70f );
		colors[ ImGuiCol_DockingEmptyBg ] = ImVec4( 0.07f, 0.07f, 0.07f, 1.00f );

		// Scrollbar
		colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.09f, 0.09f, 0.09f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.24f, 0.24f, 0.24f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.33f, 0.33f, 0.33f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.44f, 0.44f, 0.44f, 1.00f );

		colors[ ImGuiCol_Text ] = ImVec4( 0.86f, 0.86f, 0.86f, 1.00f );
		colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.40f, 0.40f, 0.40f, 1.00f );

		style.WindowRounding = 4.0f;
		style.FrameRounding = 3.0f;
		style.PopupRounding = 4.0f;
		style.ScrollbarRounding = 4.0f;
		style.GrabRounding = 3.0f;
		style.TabRounding = 3.0f;

		style.WindowPadding = ImVec2( 8.0f, 8.0f );
		style.FramePadding = ImVec2( 6.0f, 4.0f );
		style.ItemSpacing = ImVec2( 6.0f, 4.0f );
		style.ItemInnerSpacing = ImVec2( 4.0f, 4.0f );
		style.IndentSpacing = 14.0f;

		style.WindowBorderSize = 1.0f;
		style.FrameBorderSize = 0.0f;
		style.PopupBorderSize = 1.0f;

		Fonts::Initialize( );
		ImGui::GetIO( ).FontDefault = Fonts::sDefault;

	}
	void Editor::set_flags_recursive( ImGuiDockNode* node, ImGuiDockNodeFlags flags ) {
		if (!node) return;
		node->LocalFlags |= flags;
		set_flags_recursive( node->ChildNodes[ 0 ], flags );
		set_flags_recursive( node->ChildNodes[ 1 ], flags );
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