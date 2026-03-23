#include "editor.hpp"
<<<<<<< HEAD
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
=======
>>>>>>> f8ece7c (Editor in works)

namespace lum {

	void Editor::Run( ) {

<<<<<<< HEAD
		ImGui::GetIO( ).ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		while (mEngine.IsRunning( )) {

			float64 delta = mEngine.GetDeltaTime( );

			mEngine.BeginFrame( );
			begin_imgui( );

			mEngine.GetRender( ).mRenderer.UpdateCamera( mCamera.Update( delta ) );

			mEngine.Tick( );

			DrawViewport( );

			mEngine.EndFrame( );
			end_imgui( );
			mEngine.GetPlatform( ).mRenderDevice->SwapBuffers( );
=======
		while (mEngine.IsRunning( )) {

			mEngine.BeginFrame( );

			mEngine.Tick( );

			mEngine.EndFrame( );
>>>>>>> f8ece7c (Editor in works)

		}

	}
	void Editor::DrawSceneHierarchy( ) {

	}
	void Editor::DrawInspector( ) {

	}
	void Editor::DrawViewport( ) {

<<<<<<< HEAD
		ImGuiID dockID = ImGui::DockSpaceOverViewport( );

		ImGui::Begin( "Inspector" );
		ImGui::End( );

		ImGui::Begin( "Viewport" );

		ImVec2 size = ImGui::GetContentRegionAvail( );

		uint32 texID = mEngine.GetPlatform( ).mRenderDevice->GetNativeHandle( mEngine.GetRender( ).mRenderer.GetScreenTexture( ) );
		ImGui::Image( ( ImTextureID ) ( uintptr_t ) texID, size, ImVec2( 0, 1 ), ImVec2( 1, 0 ) );

		ImGui::End( );

		ImGui::Begin( "ECS Inspector" );
		ImGui::End( );

		ImGui::Begin( "Console" );
		ImGui::End( );


		ImGuiID left, right;
		ImGui::DockBuilderSplitNode( dockID, ImGuiDir_Left, 0.25f, &left, &right );

		ImGuiID middle, middleRight;
		ImGui::DockBuilderSplitNode( right, ImGuiDir_Left, 0.75f, &middle, &middleRight );

		ImGuiID console, viewport;
		ImGui::DockBuilderSplitNode( middle, ImGuiDir_Down, 0.25f, &console, &viewport );

		ImGui::DockBuilderDockWindow( "Console", console );
		ImGui::DockBuilderDockWindow( "Viewport", viewport );
		ImGui::DockBuilderDockWindow( "ECS Inspector", middleRight );
		ImGui::DockBuilderDockWindow( "Inspector", left );

		ImGui::DockBuilderFinish( dockID );

	}


	void Editor::init_imgui( Window* window ) {

		ImGui::CreateContext( );
		ImGui_ImplGlfw_InitForOpenGL( static_cast< GLFWwindow* >(window->GetNativeWindow( )), true );
		ImGui_ImplOpenGL3_Init( "#version 450" );

	}
	void Editor::begin_imgui( ) {

		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplGlfw_NewFrame( );
		ImGui::NewFrame( );

	}
	void Editor::end_imgui( ) {

		ImGui::Render( );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );

=======
>>>>>>> f8ece7c (Editor in works)
	}

}