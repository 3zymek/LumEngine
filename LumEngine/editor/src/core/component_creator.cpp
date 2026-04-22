#include "core/component_creator.hpp"
#include "editor.hpp"
#include "editor_dep_manager.generated.hpp"
namespace lum::editor {

	void ComponentsCreator::Handle( FScene* scene, EntityID entity ) {

		ImVec2 center = ImGui::GetMainViewport( )->GetCenter( );
		ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
		ImGui::SetNextWindowSize( ImVec2( 900, 520 ), ImGuiCond_Appearing );
		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoResize;

		ImGui::Begin( "##LumEngineComponentsCreator" );
		ImGui::Text( "LumEngine Components Creator" );
		ImGui::Separator( );

		if (ImGui::BeginTable( "##ComponentsCreatorLayout", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV )) {

			ImGui::BeginChild( "##LeftPanel", ImVec2( 0, -mFooterHeight ), true );

			ImGui::TextDisabled( "Search components..." );
			ImGui::SetNextItemWidth( -1 );
			SearchField( "##LumEngineComponentsCreatorList", "", mComponentsFilter.Data( ), mComponentsFilter.MaxSize( ) );
			ImGui::Separator( );

			for (auto& [category, entries] : Editor::GetComponentsByCategory( )) {

				ImVec4 categoryColor = GetCategoryColor( category );

				ImGui::Spacing( );
				ImGui::TextDisabled( "%s", category.data( ) );

				ImGui::Indent( 5.0f );
				for (auto* entry : entries) {

					auto it = std::search(
						entry->mDisplayName.begin( ), entry->mDisplayName.end( ),
						mComponentsFilter.begin( ), mComponentsFilter.end( ),
						[]( char a, char b ) { return tolower( a ) == tolower( b ); }
					);

					if (it == entry->mDisplayName.end( )) continue;

					ImGui::TextColored( categoryColor, ICON_FA_CUBE );
					ImGui::SameLine( );

					if (ImGui::Selectable( entry->mDisplayName.data( ) )) {
						//auto it = std::find( mAddedComponents.begin( ), mAddedComponents.end( ), entry );
						//if (it == mAddedComponents.end( ))
						//	mAddedComponents.push_back( entry );
						//else mActionTooltip.Trigger( "Cannot add component that's already added" );

					}
				}
				ImGui::Unindent( 5.0f );
			}

			ImGui::EndChild( );

		}


		ImGui::End( );


	}

}