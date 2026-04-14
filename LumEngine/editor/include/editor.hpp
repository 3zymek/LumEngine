#pragma once

#include "engine.hpp"
#include "core/editor_camera.hpp"
#include "core/editor_console.hpp"
#include "core/file_explorer.hpp"

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

		EntityID mSelectedEntity;

		inline static std::unordered_map<uint64, detail::EditorFn> skDrawFunctions;

		void draw_viewport( float64 delta );
		void draw_menu_bar( );
		void draw_scene_inspector( );
		void draw_entity_inspector( );
		void draw_layout( );

		void init_imgui( Window* window );
		void begin_imgui( );
		void end_imgui( );

	};

}