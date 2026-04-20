#pragma once
#include "core/editor_common.hpp"
#include "core/editor_camera.hpp"
#include "core/editor_console.hpp"
#include "core/file_explorer.hpp"
#include "core/scene_inspector.hpp"
#include "core/entity_inspector.hpp"

namespace lum::editor {

	using ComponentsEntriesMap = std::unordered_map<HashedStr, EditorComponentEntry>;
	using ComponentsByCategoryMap = std::unordered_map<StringView, std::vector<EditorComponentEntry*>>;

	class Editor {
	public:

		void Initialize( );
		void Finalize( );
		void Run( ) ;
		void Update( float64 delta ) ;

		inline static const
			ComponentsEntriesMap& GetComponentsEntries( ) { return skComponentsEntries; }

		inline static const
			ComponentsByCategoryMap& GetComponentsByCategory( ) { return skComponentsByCategory; }

	private:

		Engine mEngine;
		Console mConsole;
		EditorCamera mCamera;
		FileExplorer mExplorer;
		SceneInspector mSceneInspector;
		EntityInspector mEntityInspector;

		inline static ComponentsEntriesMap skComponentsEntries;
		inline static ComponentsByCategoryMap skComponentsByCategory;

		void draw_viewport( float64 delta );
		void draw_menu_bar( );
		void draw_layout( );

		void init_imgui( Window* window );
		void set_flags_recursive( ImGuiDockNode* node, ImGuiDockNodeFlags flags );
		void begin_imgui( );
		void end_imgui( );

	};

}