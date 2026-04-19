#include "editor.hpp"
#include "editor_dep_manager.generated.hpp"
#include "core/utils/fonts.hpp"
#include "core/utils/shortcuts.hpp"
#include "core/utils/style.hpp"

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

		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 0.0f, 12.0f ) );
		ImGui::PushStyleColor( ImGuiCol_MenuBarBg, style::skBg );
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
		ImGui::PopStyleColor( );
		ImGui::PopStyleVar( );
	}
	void Editor::draw_entity_inspector( ) {

		auto* scene = mEngine.GeModuleScene( ).mSceneMgr.GetCurrentScene( );

		ImGui::Begin( "Entity Inspector" );
		if (mSceneInspector.GetSelectedEntity( ) != ecs::skNullEntity) {
			scene->mEntityMgr.ForEachComponent(
				mSceneInspector.GetSelectedEntity( ),
				[&]( ecs::detail::BasePool* pool ) {

					ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
					ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0, 0, 0, 0 ) );
					ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0, 0, 0, 0 ) );
					ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0, 0, 0, 0 ) );

					ImGui::SetNextItemOpen( true, ImGuiCond_Once );
					ImGui::PushID( HashStr( pool->GetParseName( ) ) );
					bool open = ImGui::CollapsingHeader( "##hidden" );
					ImGui::PopID( );

					ImGui::PopStyleColor( 4 );

					bool hovered = ImGui::IsItemHovered( );
					ImVec2 min = ImGui::GetItemRectMin( );
					ImVec2 textPos = ImVec2( min.x + 5.0f, min.y );

					ccharptr chevron = open ? ICON_FA_ANGLE_DOWN : ICON_FA_ANGLE_RIGHT;

					char label[ 32 ]{};
					FormatString( label, "%s %s", chevron, pool->GetDisplayName( ).data( ) );

					ImVec4 color = GetCategoryColor( pool->GetCategoryName( ) );

					ImGui::PushFont( Fonts::sDefaultMedium );
					ImGui::GetWindowDrawList( )->AddText(
						textPos,
						hovered ? ImGui::GetColorU32( ImVec4( color.x, color.y, color.z, 1.0f ) ) : ImGui::GetColorU32( ImVec4( color.x, color.y, color.z, 0.5f ) ),
						label
					);
					ImGui::PopFont( );

					if (open) {
						skDrawFunctions[ HashStr( pool->GetParseName( ) ) ].mEditorFn( scene->mEntityMgr, mSceneInspector.GetSelectedEntity( ) );
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

			ImGuiID sceneInspector, fileExplorer;
			ImGui::DockBuilderSplitNode( left, ImGuiDir_Up, 0.5f, &sceneInspector, &fileExplorer );

			ImGui::DockBuilderDockWindow( "Console", console );
			ImGui::DockBuilderDockWindow( "Viewport", viewport );
			ImGui::DockBuilderDockWindow( "Scene Inspector", sceneInspector );
			ImGui::DockBuilderDockWindow( "Entity Inspector", right );
			ImGui::DockBuilderDockWindow( "File Explorer", fileExplorer );

			ImGui::DockBuilderFinish( dockID );

			set_flags_recursive( ImGui::DockBuilderGetNode( dockID ), ImGuiDockNodeFlags_NoWindowMenuButton );
		}

	}
	void Editor::init_imgui( Window* window ) {

		ImGui::CreateContext( );

		ImGui_ImplGlfw_InitForOpenGL( static_cast< GLFWwindow* >(window->GetNativeWindow( )), true );
		ImGui_ImplOpenGL3_Init( "#version 450" );

		style::ApplyTheme( );

		Fonts::Initialize( );
		ImGui::GetIO( ).FontDefault = Fonts::sDefaultSmall;

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