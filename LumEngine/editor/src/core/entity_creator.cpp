//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI dialog for creating new entities with a custom component set.
//
//=============================================================================//

#include "core/entity_creator.hpp"
#include "editor.hpp"
#include "editor_dep_manager.generated.hpp"

namespace lum::editor {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void EntityCreator::Handle( FScene* scene ) {

		mActionTooltip.Draw( );
		if (!bOpened) return;

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
			handle_left_panel( );

			ImGui::TableNextColumn( );
			handle_right_panel( );

			ImGui::EndTable( );
		}

		handle_footer( scene );

		ImGui::End( );

	}


	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void EntityCreator::handle_left_panel( ) {

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
					auto it = std::find( mAddedComponents.begin( ), mAddedComponents.end( ), entry );
					if (it != mAddedComponents.end( )) continue;
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
					mAddedComponents.push_back( entry );

			}
			ImGui::Unindent( 5.0f );
		}

		ImGui::EndChild( );

	}
	void EntityCreator::handle_right_panel( ) {

		ImGui::BeginChild( "##RightPanel", ImVec2( 0, -mFooterHeight ), true );

		ImGui::TextDisabled( "Name" );
		ImGui::SetNextItemWidth( -1 );
		ImGui::InputText( "##EntityName", mEntityName.Data( ), mEntityName.MaxSize( ) );

		auto it = std::find( mAddedComponents.begin( ), mAddedComponents.end( ), mNameEntry );
		if (mEntityName.Length( ) > 0) {
			if(!mNameEntry)
				mNameEntry = FindComponentEntry( "Name" );
			if (it == mAddedComponents.end( )) {
				mAddedComponents.push_back( mNameEntry );
			}
		}
		else {
			if(it != mAddedComponents.end())
				mAddedComponents.erase( it );
		}

		ImGui::Separator( );

		ImGui::TextDisabled( "Added components" );
		ImGui::BeginChild( "##AddedList", ImVec2( 0, 150 ), true );
		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0, 0, 0, 0 ) );
		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 2, 2 ) );

		char buff[ 6 ]{};
		for (int32 i = 0; i < mAddedComponents.size( ); ) {
			FormatString( buff, "%c##%d", '-', i );

			ImGui::SetNextItemWidth( 16.0f );
			if (ImGui::Button( buff, ImVec2( 19, 19 ) )) {
				mAddedComponents.erase( mAddedComponents.begin( ) + i );
				continue;
			}
			ImGui::SameLine( );
			const EditorComponentEntry* entry = mAddedComponents[ i ];
			ImGui::TextColored( GetCategoryColor( entry->mCategoryName ), entry->mDisplayName.data( ) );

			++i;
		}

		ImGui::PopStyleVar( );
		ImGui::PopStyleColor( );

		if (mAddedComponents.empty( )) {
			ImGui::TextDisabled( "Click component on the left to add" );
		}

		ImGui::EndChild( );

		ImGui::Separator( );

		ImGui::TextDisabled( "Parent entity" );
		static char parent[ 64 ]{};
		ImGui::InputText( "##Parent", parent, sizeof( parent ), ImGuiInputTextFlags_ReadOnly );
		ImGui::SameLine( );
		ImGui::Button( "Pick" );

		ImGui::TextDisabled( "Transform is NOT inherited (for now)" );

		ImGui::EndChild( );

	}
	void EntityCreator::handle_footer( FScene* scene ) {

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
		if (ImGui::Button( "Create", ImVec2( 80, 32 ) )) {

			handle_creation( scene );

		}
		ImGui::PopStyleColor( 4 );
		ImGui::PopStyleVar( ); // Frame rounding

	}
	void EntityCreator::handle_creation( FScene* scene ) {

		Entity newEntity;
		const EditorComponentEntry* entry = FindComponentEntry( "Name" );
		auto it = std::find( mAddedComponents.begin( ), mAddedComponents.end( ), entry );
		if (it != mAddedComponents.end( )) {
			scene->mEntityMgr.AddComponent( newEntity, CName{ .mName = mEntityName.Data( ) } );
			mAddedComponents.erase( it );
		}
		for (auto& comp : mAddedComponents) {
			comp->mCreateFn( scene->mEntityMgr, newEntity.GetID( ) );
		}
		scene->mEntities.push_back( newEntity.GetID( ) );
		bOpened = false;
		mAddedComponents.clear( );

	}
	void EntityCreator::handle_closing( ) {
		bOpened = false;
		mAddedComponents.clear( );
		mComponentsFilter.Clear( );
		mEntityName.Clear( );
	}

}