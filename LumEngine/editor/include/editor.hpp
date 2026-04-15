#pragma once

#include "engine.hpp"
#include "core/editor_camera.hpp"
#include "core/editor_console.hpp"
#include "core/file_explorer.hpp"
#include "core/scene_inspector.hpp"

namespace lum::editor {

	namespace detail { using EditorFn = void(*)(ecs::MEntityManager&, EntityID); }

	class Editor {
	public:
		
		void Initialize( );
		void Finalize( );
		void Run( ) ;
		void Update( float64 delta ) ;

	private:
		
		Engine mEngine;
		Console mConsole;
		EditorCamera mCamera;
		FileExplorer mExplorer;
		SceneInspector mSceneInspector;

		inline static std::unordered_map<uint64, detail::EditorFn> skDrawFunctions;

		void draw_viewport( float64 delta );
		void draw_menu_bar( );
		void draw_entity_inspector( );
		void draw_layout( );

		void init_imgui( Window* window );
		void set_flags_recursive( ImGuiDockNode* node, ImGuiDockNodeFlags flags );
		void begin_imgui( );
		void end_imgui( );

	};

}