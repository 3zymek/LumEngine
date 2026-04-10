#include "editor.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "entity/components/name.hpp"
#include "editor_dep_manager.generated.hpp"

namespace lum {

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

		auto* scene = mEngine.GeModuleScene( ).mSceneMgr.GetCurrentScene( );
		auto& entities = scene->mEntities;

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
		}

		ImGui::Begin( "File Explorer" );
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator( AssetLoader::GetProjectRoot( ) )) {
			if (entry.is_regular_file( )) {
				ImGui::CollapsingHeader( entry.path( ).filename( ).string().data() );
			}
		}
		ImGui::End( ); // File Explorer


		ImGui::Begin( "Viewport" );
		ImVec2 available = ImGui::GetContentRegionAvail( );
		mCamera.SetAspectRatio( ( float32 ) available.x / ( float32 ) available.y );
		mEngine.GetModuleRender( ).mRenderer.UpdateCamera( mCamera.Update( delta ) );
		uint32 texID = mEngine.GetPlatform( ).mRenderDevice->GetNativeHandle(
			mEngine.GetModuleRender( ).mRenderer.GetFrameTexture( )
		);
		ImGui::Image( ( ImTextureID ) texID, available, ImVec2( 0, 1 ), ImVec2( 1, 0 ) );
		ImGui::End( ); // Viewport


		ImGui::Begin( "Scene Inspector" );
		for (auto& entity : entities) {
			bool selected = entity == mSelectedEntity;
			CName* name = scene->mEntityMgr.GetComponent<CName>( entity );
			String label;
			if (!name) label = "Entity " + ToString( entity );
			else label = name->mName.Data( );
			if (ImGui::Selectable( label.data( ), selected )) {
				mSelectedEntity = entity;
			}
		}
		ImGui::End( ); // Scene Inspector

		ImGui::Begin( "Entity Inspector" );
		if (mSelectedEntity != ecs::skNullEntity) {
			scene->mEntityMgr.ForEachComponent(
				mSelectedEntity,
				[&]( ecs::detail::BasePool* pool ) {
					if (ImGui::CollapsingHeader( pool->GetName( ).data( ) )) {
						skDrawFunctions[ HashStr( pool->GetName( ) ) ]( scene->mEntityMgr, mSelectedEntity );
					}
				} );
		}
		ImGui::End( );


		ImGui::Begin( "Console" );
		ImGui::BeginChild( "ConsoleScrollRegion", ImVec2( 0, 0 ) );
		ImGui::SetWindowFontScale( 1.5f );
		auto& logs = Logger::Get( ).GetLogs( );
		for (auto& log : logs) {
			char time[ 16 ]{};
			Logger::Get( ).FormatTime( log.mTime, time );
			ImGui::TextUnformatted( "[ " ); ImGui::SameLine( );
			ImGui::TextDisabled( time ); ImGui::SameLine( );
			ImGui::TextUnformatted( " ]" ); ImGui::SameLine( );
			if (log.mSeverity == LogSeverity::Info)
				ImGui::Text( log.mMessage.c_str( ) );
			else if (log.mSeverity == LogSeverity::Warn)
				ImGui::TextColored( ImVec4( 1.0f, 0.85f, 0.0f, 1.0f ), log.mMessage.c_str( ) );
			else if (log.mSeverity == LogSeverity::Error)
				ImGui::TextColored( ImVec4( 1.0f, 0.1f, 0.1f, 1.0f ), log.mMessage.c_str( ) );
			else if (log.mSeverity == LogSeverity::Fatal)
				ImGui::TextColored( ImVec4( 1.0f, 0.0f, 0.4f, 1.0f ), log.mMessage.c_str( ) );
			else if (log.mSeverity == LogSeverity::Debug)
				ImGui::TextColored( ImVec4( 0.50f, 0.0f, 0.80f, 1.0f ), log.mMessage.c_str( ) );
		}
		ImGui::EndChild( );
		ImGui::End( ); // Console
	}



	void Editor::init_imgui( Window* window ) {

		ImGui::CreateContext( );
		ImGui_ImplGlfw_InitForOpenGL( static_cast< GLFWwindow* >(window->GetNativeWindow( )), true );
		ImGui_ImplOpenGL3_Init( "#version 450" );

		LUM_LOG_ERROR( "ERROR" );
		LUM_LOG_INFO( "INFO" );
		LUM_LOG_DEBUG( "DEBUG" );
		LUM_LOG_WARN( "WARN" );
		LUM_LOG_FATAL( "FATAL" );
		ImGuiStyle& style = ImGui::GetStyle( );
		ImVec4* colors = style.Colors;

		// Tło główne
		colors[ ImGuiCol_WindowBg ] = ImVec4( 0.09f, 0.09f, 0.09f, 1.00f );
		colors[ ImGuiCol_ChildBg ] = ImVec4( 0.11f, 0.11f, 0.11f, 1.00f );
		colors[ ImGuiCol_PopupBg ] = ImVec4( 0.13f, 0.13f, 0.13f, 1.00f );

		// Nagłówki / paski tytułu
		colors[ ImGuiCol_TitleBg ] = ImVec4( 0.07f, 0.07f, 0.07f, 1.00f );
		colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
		colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.07f, 0.07f, 0.07f, 1.00f );

		// Ramki / bordery
		colors[ ImGuiCol_Border ] = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
		colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );

		// Pola tekstowe / pola input
		colors[ ImGuiCol_FrameBg ] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
		colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.19f, 0.19f, 0.19f, 1.00f );
		colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.24f, 0.24f, 0.24f, 1.00f );

		// Akcent (niebieski jak w Blenderze)
		colors[ ImGuiCol_CheckMark ] = ImVec4( 0.33f, 0.55f, 0.86f, 1.00f );
		colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.33f, 0.55f, 0.86f, 1.00f );
		colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.44f, 0.65f, 0.95f, 1.00f );

		// Przyciski
		colors[ ImGuiCol_Button ] = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
		colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.33f, 0.55f, 0.86f, 1.00f );
		colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.25f, 0.45f, 0.75f, 1.00f );

		// Nagłówki (TreeNode, CollapsingHeader)
		colors[ ImGuiCol_Header ] = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
		colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.26f, 0.26f, 0.26f, 1.00f );
		colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.33f, 0.55f, 0.86f, 1.00f );

		// Separator
		colors[ ImGuiCol_Separator ] = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
		colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.33f, 0.55f, 0.86f, 1.00f );
		colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.44f, 0.65f, 0.95f, 1.00f );

		// Tabs
		colors[ ImGuiCol_Tab ] = ImVec4( 0.11f, 0.11f, 0.11f, 1.00f );
		colors[ ImGuiCol_TabHovered ] = ImVec4( 0.26f, 0.26f, 0.26f, 1.00f );
		colors[ ImGuiCol_TabActive ] = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
		colors[ ImGuiCol_TabUnfocused ] = ImVec4( 0.09f, 0.09f, 0.09f, 1.00f );
		colors[ ImGuiCol_TabUnfocusedActive ] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );

		// Dockspace
		colors[ ImGuiCol_DockingPreview ] = ImVec4( 0.33f, 0.55f, 0.86f, 0.70f );
		colors[ ImGuiCol_DockingEmptyBg ] = ImVec4( 0.07f, 0.07f, 0.07f, 1.00f );

		// Scrollbar
		colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.09f, 0.09f, 0.09f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.24f, 0.24f, 0.24f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.33f, 0.33f, 0.33f, 1.00f );
		colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.44f, 0.44f, 0.44f, 1.00f );

		// Tekst
		colors[ ImGuiCol_Text ] = ImVec4( 0.86f, 0.86f, 0.86f, 1.00f );
		colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.40f, 0.40f, 0.40f, 1.00f );

		// Styl zaokrągleń i odstępów (jak Blender)
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

		ImGui::GetIO( ).Fonts->AddFontFromFileTTF( "C:/Windows/Fonts/segoeui.ttf", 15.0f );
		ImGui::GetIO( ).FontGlobalScale = 1.3f;

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