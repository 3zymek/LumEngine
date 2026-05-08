#include "editor.hpp"
#include "editor_dep_manager.generated.hpp"
#include "core/utils/fonts.hpp"
#include "core/utils/shortcuts.hpp"
#include "core/utils/style.hpp"

namespace lum::editor {

	void Editor::Initialize( ) {

		mEngine.Initialize( "C:/Users/szymek/Desktop/lumen_assets" ); mEngine.SetScene( "scene.lsc" );

		init_imgui( &mEngine.GetModulePlatform( ).mWindow );

		RegisterEditorComponents( skComponentsEntries );
		for (auto& [hash, entry] : skComponentsEntries) {
			skComponentsByCategory[ entry.mCategoryName ].emplace_back( &entry );
		}

		mWindow = &mEngine.GetModulePlatform( ).mWindow;
		mRenderDevice = mEngine.GetModulePlatform( ).mRenderDevice;
		mRenderer = &mEngine.GetModuleRender( ).mRenderer;
		
		mSceneInspector.Initialize( &mEngine.GetEventBus( ) );

	}

	void Editor::Finalize( ) {
		mEngine.Finalize( );
	}

	void Editor::Run( ) {

		while (mEngine.IsRunning( )) {

			float64 delta = mEngine.GetDeltaTime( );

			mEngine.BeginFrame( );
			begin_imgui( );

			mEngine.Tick( );

			Update( delta );

			auto& meshMgr = mEngine.GetModuleResource( ).mMeshMgr;
			mEngine.GeModuleScene( ).mSceneMgr.GetCurrentScene( )->mEntityMgr.Each<CStaticMesh>(
				[&]( CStaticMesh& mesh ) {
					if (mesh.mPath != mesh.mLastLoadedPath) {
						mesh.mHandle = meshMgr.CreateStatic( mesh.mPath );
						mesh.mLastLoadedPath = mesh.mPath;
					}
				} 
			);

			mEngine.EndFrame( );
			end_imgui( );
			mEngine.GetModulePlatform( ).mRenderDevice->SwapBuffers( );

		}

	}
	void Editor::Update( float64 delta ) {

		mCurrentScene = mEngine.GeModuleScene( ).mSceneMgr.GetCurrentScene( );

		draw_layout( );
		draw_menu_bar( );
		mViewport.Update( delta, mRenderer->GetFrameTexture( ), mRenderDevice, mWindow, mRenderer );
		mSceneInspector.Update( mCurrentScene );
		mEntityInspector.Update( mSceneInspector.GetSelectedEntity( ), mCurrentScene );

		mExplorer.Update( AssetLoader::GetProjectRoot( ) );

		mConsole.Update( );

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
					mEngine.GetModulePlatform( ).mWindow.Close( );
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
			ImGui::DockBuilderDockWindow( "Scene", sceneInspector );
			ImGui::DockBuilderDockWindow( "Entity", right );
			ImGui::DockBuilderDockWindow( "File System", fileExplorer );

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
		ImGui::GetIO( ).ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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