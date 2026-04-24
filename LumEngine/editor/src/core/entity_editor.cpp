//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI dialog for inspecting and editing components of an existing entity.
//
//=============================================================================//

#include "core/entity_editor.hpp"
#include "editor.hpp"
#include "editor_dep_manager.generated.hpp"

namespace lum::editor {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void EntityEditor::Handle( FScene* scene, EntityID entityID ) {

		mActionTooltip.Draw( );
		if (!bOpened) return;
		
		if (bFindComponents) {
			scene->mEntityMgr.ForEachComponent(
				entityID,
				[&]( int32 typeID, ecs::BasePool* pool ) {
					for (auto& [category, entries] : Editor::GetComponentsByCategory( )) {
						for (auto& entry : entries) {
							if (entry->mTypeID == typeID)
								mEntityComponents.push_back( { entry, skOldComponent } );
						}
					}
				}
			);

			bFindComponents = false;
		}

		ImVec2 center = ImGui::GetMainViewport( )->GetCenter( );
		ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
		ImGui::SetNextWindowSize( ImVec2( 900, 520 ), ImGuiCond_Appearing );
		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoResize;

		ImGui::Begin( "##EntityCreator", &bOpened, flags );
		ImGui::Text( "LumEngine Entity Creator" );
		ImGui::Separator( );

		if (ImGui::BeginTable( "##EntityCreatorLayout", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV )) {

			ImGui::TableSetupColumn( "Left", ImGuiTableColumnFlags_WidthStretch, 0.5f );
			ImGui::TableSetupColumn( "Right", ImGuiTableColumnFlags_WidthStretch, 0.5f );

			ImGui::TableNextColumn( );
			handle_left_panel( entityID );

			ImGui::TableNextColumn( );
			handle_right_panel( entityID );

			ImGui::EndTable( );
		}

		handle_footer( scene, entityID );

		ImGui::End( );

	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void EntityEditor::handle_left_panel( EntityID entityID ) {

		ImGui::BeginChild( "##LeftPanel", ImVec2( 0, -mFooterHeight ), true );

		ImGui::TextDisabled( "Search components..." );
		ImGui::SetNextItemWidth( -1 );
		SearchField( "##LumEngineEntiyCreatorList", "", mComponentsFilter.Data( ), mComponentsFilter.MaxSize( ) );
		ImGui::Separator( );

		for (auto& [category, entries] : Editor::GetComponentsByCategory( )) {

			ImVec4 categoryColor = GetCategoryColor( category );

			ImGui::Spacing( );
			ImGui::TextDisabled( "%s", category.data( ) );

			ImGui::Indent( 5.0f );
			for (auto* entry : entries) {

				{
					auto it = std::find_if(
						mEntityComponents.begin( ),
						mEntityComponents.end( ),
						[&]( const auto& pair ) { return pair.first == entry; }
					);
					if (it != mEntityComponents.end( )) continue;
				}

				{
					auto it = std::search(
						entry->mDisplayName.begin( ), entry->mDisplayName.end( ),
						mComponentsFilter.begin( ), mComponentsFilter.end( ),
						[]( char a, char b ) { return tolower( a ) == tolower( b ); }
					);
					if (it == entry->mDisplayName.end( )) continue;
				}

				ImGui::TextColored( categoryColor, ICON_FA_CUBE );
				ImGui::SameLine( );

				if (ImGui::Selectable( entry->mDisplayName.data( ) ))
					mEntityComponents.push_back( { entry, skNewComponent } );

			}
			ImGui::Unindent( 5.0f );
		}

		ImGui::EndChild( );

	}
	void EntityEditor::handle_right_panel( EntityID entityID ) {

		ImGui::BeginChild( "##RightPanel", ImVec2( 0, -mFooterHeight ), true );
		ImGui::TextDisabled( "Entity components" );
		ImGui::BeginChild( "##AddedList", ImVec2( 0, 150 ), true );
		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0, 0, 0, 0 ) );
		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2, 2 ) );

		char buff[ 6 ]{};
		for (int32 i = 0; i < mEntityComponents.size( ); i++) {
			FormatString( buff, "%c##%d", '-', i );

			std::pair<const EditorComponentEntry*, bool>& pair = mEntityComponents[ i ];

			if (pair.second == skNewComponent) continue;

			ImGui::SetNextItemWidth( 16.0f );
			if (ImGui::Button( buff, ImVec2( 19, 19 ) )) {

				auto it = mEntityComponents.begin( ) + i;
				mComponentsToDelete.push_back( it->first );
				mEntityComponents.erase( it );
				continue;

			}
			ImGui::SameLine( );
			ImGui::TextColored( GetCategoryColor( pair.first->mCategoryName ), pair.first->mDisplayName.data( ) );

		}

		ImGui::PopStyleVar( );
		ImGui::PopStyleColor( );

		ImGui::EndChild( );

		ImGui::Separator( );

		ImGui::TextDisabled( "Newly added components" );
		ImGui::PushStyleColor( ImGuiCol_Border, ImVec4( 0.2f, 0.7f, 0.3f, 0.6f ) );
		ImGui::BeginChild( "##AddingList", ImVec2( 0, 150 ), true );
		ImGui::PopStyleColor( );
		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0, 0, 0, 0 ) );
		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2, 2 ) );

		for (int32 i = 0; i < mEntityComponents.size( ); i++) {
			FormatString( buff, "%c##%d", '-', i );

			std::pair<const EditorComponentEntry*, bool>& pair = mEntityComponents[ i ];

			if (pair.second == skOldComponent) continue;

			ImGui::SetNextItemWidth( 16.0f );
			if (ImGui::Button( buff, ImVec2( 19, 19 ) )) {

				mEntityComponents.erase( mEntityComponents.begin( ) + i );
				continue;

			}
			ImGui::SameLine( );
			ImGui::TextColored( GetCategoryColor( pair.first->mCategoryName ), pair.first->mDisplayName.data( ) );

		}

		ImGui::PopStyleVar( );
		ImGui::PopStyleColor( );

		ImGui::EndChild( );

		ImGui::Separator( );

		ImGui::EndChild( );

	}
	void EntityEditor::handle_footer( FScene* scene, EntityID entityID ) {

		float32 avail = ImGui::GetContentRegionAvail( ).x;
		float32 btnW = 80.0f * 2 + ImGui::GetStyle( ).ItemSpacing.x;
		ImGui::SetCursorPosX( (avail - btnW) * 0.5f );

		ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 8.0f );
		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.18f, 0.18f, 0.18f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.25f, 0.25f, 0.25f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.30f, 0.30f, 0.30f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0.7f, 0.7f, 0.7f, 1.0f ) );
		if (ImGui::Button( "Cancel", ImVec2( 80, 32 ) )) {

			handle_closing( );

		}
		ImGui::PopStyleColor( 4 );

		ImGui::SameLine( );

		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.36f, 0.31f, 0.83f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.42f, 0.37f, 0.88f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.30f, 0.26f, 0.75f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0.92f, 0.90f, 1.00f, 1.0f ) );
		if (ImGui::Button( "Apply", ImVec2( 80, 32 ) )) {

			handle_apply( scene, entityID );

		}
		ImGui::PopStyleColor( 4 );
		ImGui::PopStyleVar( ); // Frame rounding

	}
	void EntityEditor::handle_apply( FScene* scene, EntityID entityID ) {

		for (auto& pair : mEntityComponents) {

			if (pair.second == skOldComponent) continue;

			pair.first->mCreateFn( scene->mEntityMgr, entityID );

		}

		for (auto& comp : mComponentsToDelete) {

			comp->mDeleteFn( scene->mEntityMgr, entityID );

		}

		handle_closing( );

	}
	void EntityEditor::handle_closing( ) {
		bOpened = false;
		mEntityComponents.clear( );
		mComponentsFilter.Clear( );
		mComponentsToDelete.clear( );
		bFindComponents = true;
	}

}