#include "core/scene_inspector.hpp"
#include "scene/scene_manager.hpp"
#include "entity/components/name.hpp"
#include "core/utils/style.hpp"
#include "core/utils/fonts.hpp"
#include "editor.hpp"
#include "editor_dep_manager.generated.hpp"
namespace lum::editor {

	void SceneInspector::Update( FScene* scene ) {

		auto& entities = scene->mEntities;

		ImGui::Begin( "Scene" );

		if (ImGui::Button( ICON_FA_PLUS )) {
			bEntityCreatorOpened = true;
		}
		ImGui::SameLine( );

		ImGui::PushItemWidth( -1 );
		SearchField( "##Entities search", "Filter entities...", mEntitiesFilter.Data( ), mEntitiesFilter.MaxSize( ) );
		ImGui::PopItemWidth( );

		ImGui::Separator( );

		for (usize i = 0; i < entities.size( ); i++) {

			auto& entity = entities[ i ];

			bool selected = (mSelectedEntity == entity);
			CName* name = scene->mEntityMgr.GetComponent<CName>( entity );
			char label[ 64 ]{};
			if (!name || name->mName.Length( ) == 0)
				FormatString( label, "%s %llu", "Entity", entity );
			else
				FormatString( label, "%s", name->mName.Data( ) );

			float32 height = ImGui::GetTextLineHeightWithSpacing( );
			ImVec2 pos = ImGui::GetCursorScreenPos( );
			float32 width = ImGui::GetContentRegionAvail( ).x;
			bool hovered = ImGui::IsMouseHoveringRect( pos, ImVec2( pos.x + width, pos.y + height ) );

			if (entity == mSelectedEntity)
				DrawRowBackground( style::skItemSelected );
			else if (hovered)
				DrawRowBackground( style::skItemHovered );
			else
				DrawRowBackground(
					(i % 2 != 0)
					? style::skRowAlt
					: style::skBg
				);

			ImGui::PushStyleColor( ImGuiCol_Header, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleColor( ImGuiCol_HeaderHovered, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleColor( ImGuiCol_HeaderActive, ImVec4( 0, 0, 0, 0 ) );
			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 12.0f, 4.0f ) );

			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + (height - ImGui::GetTextLineHeight( )) * 0.5f );

			if (ImGui::Selectable( label, selected, ImGuiSelectableFlags_None, ImVec2( 0, height ) )) {
				mSelectedEntity = entity;
			}
			if (selected) {
				ImVec2 min = ImGui::GetItemRectMin( );
				ImVec2 max = ImGui::GetItemRectMax( );
				ImGui::GetWindowDrawList( )->AddRectFilled(
					ImVec2( pos.x + 2.0f, pos.y + 2.0f ),
					ImVec2( pos.x + 5.0f, pos.y + 16.5f ),
					ImGui::ColorConvertFloat4ToU32( style::skAccentActive ),
					1.0f
				);
			}

			ImGui::PopStyleVar( );
			ImGui::PopStyleColor( 3 );

		}
		ImGui::End( ); // Scene Inspector

		if (bEntityCreatorOpened)
			handle_entity_creator( );

	}

	void SceneInspector::handle_entity_creator( ) {
		
		ImVec2 center = ImGui::GetMainViewport( )->GetCenter( );
		ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
		ImGui::SetNextWindowSize( ImVec2( 750, 700 ), ImGuiCond_Appearing );
		
		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoDocking;
		
		
		ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 2.0f );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 0.0f, 0.0f ) );
		ImGui::Begin( "##EntityCreator", &bEntityCreatorOpened, flags );
		ImGui::PopStyleVar( 3 );
		
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 16.0f, 16.0f ) );
		
		ImGui::SetCursorPos( ImVec2( 16.0f, 16.0f ) );
		ImGui::PushFont( Fonts::sDefaultMedium );
		ImGui::AlignTextToFramePadding( );
		ImGui::Text( "LumEngine Entity Creator" );
		ImGui::SameLine( ImGui::GetWindowWidth( ) - 45.0f );
		
		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		if (ImGui::Button( ICON_FA_TIMES )) {
			bEntityCreatorOpened = false;
		}
		ImGui::PopStyleColor( );
		ImGui::PopFont( );
		
		ImVec2 cursorPos = ImGui::GetCursorScreenPos( );
		float32 width = ImGui::GetWindowWidth( );
		ImGui::GetWindowDrawList( )->AddLine(
			cursorPos,
			ImVec2( cursorPos.x + width, cursorPos.y ),
			ImGui::ColorConvertFloat4ToU32( style::skBorder ),
			1.0f
		);
		
		ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 2.0f );
		ImGui::PushStyleColor( ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f) );
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 1.0f );
		ImGui::BeginChild( "##EntityCreatorComponentsList", ImVec2( 300.0f, ImGui::GetContentRegionAvail( ).y ), true, ImGuiChildFlags_Borders );
		ImGui::PopStyleColor( );
		ImGui::PopStyleVar( );

		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 10.0f, 10.0f ) );
		ImGui::SetNextItemWidth( -1 );
		ImGui::PushFont( Fonts::sDefaultMedium );
		SearchField( "##EntityCreatorSearch", "Search components...", mComponentsFilter.Data( ), mComponentsFilter.MaxSize( ) );
		ImGui::PopFont( );

		ImVec2 cursorPos2 = ImGui::GetCursorScreenPos( );
		ImVec2 windowPos = ImGui::GetCurrentWindow( )->ParentWindow->Pos;
		ImGui::GetCurrentWindow()->ParentWindow->DrawList->AddLine(
			ImVec2( windowPos.x, cursorPos2.y ),
			ImVec2( windowPos.x + width, cursorPos2.y ),
			ImGui::ColorConvertFloat4ToU32( style::skBorder ),
			1.0f
		);
		ImGui::Dummy( ImVec2( 0.0f, 1.0f ));

		FCollapsingHeaderArgs args;
		for (auto& [category, entries] : Editor::GetComponentsByCategory( )) {
			
			ImVec4 catColor = GetCategoryColor( category );

			bool opened = false;

			args.mLabel = category;
			args.mID = HashStr( category );
			args.mFont = Fonts::sDefaultBig;
			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 8.0f, 1.0f ) );
			ImGui::Indent( 2.0f );
			CollapsingHeaderCustom( args, opened );
			ImGui::Unindent( 2.0f );
			ImGui::PopStyleVar( );

			if (opened) {
				ImGui::PushFont( Fonts::sDefaultMedium );
				for (auto& comp : entries) {
					
					ImGui::Indent( 6.0f );
					ImGui::TextColored( catColor, ICON_FA_PLUS );
					ImGui::SameLine( );
					ImGui::Selectable( comp->mDisplayName.data() ); 
					ImGui::Unindent( 6.0f );

				}
				ImGui::PopFont( );
			}

		}

		ImGui::PopStyleVar( );
		ImGui::EndChild( );

		ImGui::SameLine( );

		ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 2.0f );
		ImGui::PushStyleColor( ImGuiCol_ChildBg, style::skBg );
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 1.0f );
		ImGui::BeginChild( "##EntityCreatorMenu" );
		ImGui::PopStyleColor( );
		ImGui::PopStyleVar( );


		ImGui::EndChild( );

		ImGui::PopStyleVar( );
		ImGui::End( );

	}

} // namespace lum::editor