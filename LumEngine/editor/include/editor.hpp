#pragma once

#include "engine.hpp"
#include "editor_camera.hpp"

namespace lum {

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
		EditorCamera mCamera;
		glm::vec2 mViewportSize = glm::vec2( 0, 0 );
		bool bLayoutInitialized = false;

		EntityID mSelectedEntity;

		static std::unordered_map<uint64, detail::EditorFn> skDrawFunctions;

		void init_imgui( Window* window );
		void begin_imgui( );
		void end_imgui( );

	};

}