#include "core/scene_inspector.hpp"
#include "scene/scene_manager.hpp"
#include "entity/components/name.hpp"
#include "core/utils/style.hpp"
#include "core/utils/fonts.hpp"
#include "editor.hpp"
#include "core/editor_common.hpp"
#include "editor_dep_manager.generated.hpp"
#include "entity/ecs_common.hpp"
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
			handle_entity_creator( scene );

		mActionTooltip.Draw( );

	}

	void SceneInspector::handle_entity_creator( FScene* scene ) {

		static std::vector<const EditorComponentEntry*> mAddedComponents;

		ImVec2 center = ImGui::GetMainViewport( )->GetCenter( );
		ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
		ImGui::SetNextWindowSize( ImVec2( 900, 520 ), ImGuiCond_Appearing );

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoResize;

		ImGui::Begin( "##EntityCreator", &bEntityCreatorOpened, flags );

		// ── HEADER ───────────────────────────────────────────────────────────────
		ImGui::Text( "Create entity" );
		ImGui::Separator( );

		const float32 footerHeight = 36.0f;

		if (ImGui::BeginTable( "##EntityCreatorLayout", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV )) {

			ImGui::TableSetupColumn( "Left", ImGuiTableColumnFlags_WidthStretch, 0.5f );
			ImGui::TableSetupColumn( "Right", ImGuiTableColumnFlags_WidthStretch, 0.5f );

			// ================= LEFT =================
			ImGui::TableNextColumn( );
			ImGui::BeginChild( "##LeftPanel", ImVec2( 0, -footerHeight ), true );

			ImGui::TextDisabled( "Search components..." );
			static char search[ 64 ]{};
			ImGui::SetNextItemWidth( -1 );
			SearchField( "##LumEngineEntiyCreatorList", "", search, sizeof( search ) );
			ImGui::Separator( );

			for (auto& [category, entries] : Editor::GetComponentsByCategory( )) {

				ImVec4 categoryColor = GetCategoryColor( category );

				ImGui::Spacing( );
				ImGui::TextDisabled( "%s", category.data( ) );

				ImGui::Indent( 5.0f );
				for (auto* entry : entries) {

					ImGui::TextColored( categoryColor, ICON_FA_CUBE );
					ImGui::SameLine( );

					if (ImGui::Selectable( entry->mDisplayName.data( ) )) {
						auto it = std::find( mAddedComponents.begin( ), mAddedComponents.end( ), entry );
						if (it == mAddedComponents.end( ))
							mAddedComponents.push_back( entry );
						else mActionTooltip.Trigger( "Cannot add component that's already added" );

					}
				}
				ImGui::Unindent( 5.0f );
			}

			ImGui::EndChild( );

			// ── Right ─────────────────────────
			ImGui::TableNextColumn( );

			ImGui::BeginChild( "##RightPanel", ImVec2( 0, -footerHeight ), true );

			// NAME
			ImGui::TextDisabled( "Name" );
			static char entityName[ 64 ]{};
			ImGui::SetNextItemWidth( -1 );
			ImGui::InputText( "##EntityName", entityName, sizeof( entityName ) );

			ImGui::Separator( );

			// ADDED COMPONENTS
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

			// PARENT
			ImGui::TextDisabled( "Parent entity" );
			static char parent[ 64 ]{};
			ImGui::InputText( "##Parent", parent, sizeof( parent ), ImGuiInputTextFlags_ReadOnly );
			ImGui::SameLine( );
			ImGui::Button( "Pick" );

			ImGui::TextDisabled( "Transform is NOT inherited (for now)" );

			ImGui::EndChild( );

			ImGui::EndTable( );
		}

		// ── FOOTER ────────────────────────────────────────────────────────────────
		float32 avail = ImGui::GetContentRegionAvail( ).x;
		float32 btnW = 80.0f * 2 + ImGui::GetStyle( ).ItemSpacing.x;
		ImGui::SetCursorPosX( (avail - btnW) * 0.5f );

		ImGui::PushStyleVar( ImGuiStyleVar_FrameRounding, 8.0f );

		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.18f, 0.18f, 0.18f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.25f, 0.25f, 0.25f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.30f, 0.30f, 0.30f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0.7f, 0.7f, 0.7f, 1.0f ) );
		if (ImGui::Button( "Cancel", ImVec2( 80, 32 ) ))
			bEntityCreatorOpened = false;
		ImGui::PopStyleColor( 4 );
		
		ImGui::SameLine( );

		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.36f, 0.31f, 0.83f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.42f, 0.37f, 0.88f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.30f, 0.26f, 0.75f, 1.0f ) );
		ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 0.92f, 0.90f, 1.00f, 1.0f ) );
		if (ImGui::Button( "Create", ImVec2( 80, 32 ) )) {
			Entity newEntity = scene->mEntityMgr.CreateEntity();
			for (auto& comp : mAddedComponents) {
				comp->mCreateFn( scene->mEntityMgr, newEntity.GetID() );
			}
		}
		ImGui::PopStyleColor( 4 );

		ImGui::PopStyleVar( );

		ImGui::End( );
	}

} // namespace lum::editor