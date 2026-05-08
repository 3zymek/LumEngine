//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: UI panel for browsing, selecting and managing scene entities.
//
//=============================================================================//

#include "core/scene_inspector.hpp"
#include "entity/components/name.hpp"
#include "core/utils/style.hpp"
#include "event/events/window_events.hpp"

namespace lum::editor {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void SceneInspector::Initialize( ev::EventBus* eventBus ) {
		
		mEntityCreator.Initialize( eventBus );

	}

	void SceneInspector::Update( Scene* scene ) {

		auto& entities = scene->mEntities;

		ImGui::Begin( "Scene" );

		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		if (ImGui::Button( ICON_FA_PLUS )) {
			mEntityCreator.Toggle( true );
		} TooltipOnHover( "Add entity" );
		ImGui::SameLine( );
		ImGui::PopStyleColor( );

		ImGui::PushItemWidth( -1 );
		SearchField( "##Entities search", "Filter entities...", mEntitiesFilter.Data( ), mEntitiesFilter.MaxSize( ) );
		ImGui::PopItemWidth( );

		ImGui::Separator( );

		for (usize i = 0; i < entities.size( ); i++) {

			if (!scene->mParents.contains( entities[ i ] ))
				draw_entity( scene, entities[ i ] );
			
		}
		ImGui::End( ); // Scene Inspector

		mEntityCreator.Handle( scene );

		mActionTooltip.Draw( );

	}

    void SceneInspector::draw_entity( Scene* scene, EntityID entity ) {

        bool bHasChildren = scene->mChildren.contains( entity );
        bool bSelected = (mSelectedEntity == entity);

        CName* displayName = scene->mEntityMgr.GetComponent<CName>( entity );
        char displayLabel[ 64 ]{};
        if (!displayName || displayName->mName.Length( ) == 0)
            FormatString( displayLabel, "%s %llu", "Entity ", entity );
        else
            FormatString( displayLabel, "%s", displayName->mName.Data( ) );

        const usize len = strlen( displayLabel );
        auto it = std::search(
            displayLabel, displayLabel + len,
            mEntitiesFilter.begin( ), mEntitiesFilter.end( ),
            []( char a, char b ) { return tolower( a ) == tolower( b ); }
        );

        if (it == displayLabel + len) return;

        auto handleDragDrop = [&]( ) {
            if (ImGui::BeginDragDropSource( )) {
                ImGui::SetDragDropPayload( "ENTITY", &entity, sizeof( EntityID ) );
                ImGui::Text( displayLabel );
                ImGui::EndDragDropSource( );
            }
            if (ImGui::BeginDragDropTarget( )) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "ENTITY" )) {
                    EntityID dragged = *( EntityID* ) payload->Data;
                    scene->AttachChild( entity, dragged );
                }
                ImGui::EndDragDropTarget( );
            }
            };

        ImGui::PushStyleColor( ImGuiCol_Header, style::skItemHovered );
        ImGui::PushStyleColor( ImGuiCol_HeaderHovered, style::skItemHovered );
        ImGui::PushStyleColor( ImGuiCol_HeaderActive, style::skItemHovered );

        if (bHasChildren) {

            bool bTreeOpened = false;
            FCollapsingHeaderArgs args{
                .mID = HashedStr(displayLabel ),
                .mLabel = displayLabel,
                .mIconOpened = ICON_FA_CHEVRON_DOWN,
                .mIconClosed = ICON_FA_CHEVRON_RIGHT,
                .mColor = ImGui::GetStyleColorVec4( ImGuiCol_Text ),
                .mColorHovered = style::skAccent,
                .mFont = Fonts::sDefaultSmall
            };

            TreeNodeCustom( args, bTreeOpened, [&]( ) {

                if (ImGui::IsItemClicked( ))
                    mSelectedEntity = entity;

                for (auto child : scene->mChildren[ entity ])
                    draw_entity( scene, child );

                            } );

            handleDragDrop( );

        }
        else {
            if (ImGui::Selectable( displayLabel, bSelected ))
                mSelectedEntity = entity;
            handleDragDrop( );
        }

        ImGui::PopStyleColor( 3 );

    }

} // namespace lum::editor