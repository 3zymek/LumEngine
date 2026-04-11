#pragma once

#include "engine.hpp"
#include "core/editor_camera.hpp"
#include "core/editor_console.hpp"

namespace lum::editor {

	namespace detail { using EditorFn = void(*)(ecs::MEntityManager&, EntityID); }

	class Editor {
	public:

		void Initialize( );
		void Finalize( );
		void Run( ) ;
		void DrawSceneHierarchy( );
		void DrawInspector( );
		void DrawViewport( float64 delta ) ;

	private:

		Engine mEngine;
		Console mConsole;
		EditorCamera mCamera;
		glm::vec2 mViewportSize = glm::vec2( 0, 0 );
		bool bLayoutInitialized = false;

		EntityID mSelectedEntity;

		inline static std::unordered_map<uint64, detail::EditorFn> skDrawFunctions;

		void init_imgui( Window* window );
		void begin_imgui( );
		void end_imgui( );

	};

}