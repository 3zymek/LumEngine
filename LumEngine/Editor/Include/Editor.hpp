#pragma once
#include "Core/EditorCommon.hpp"
#include "Core/EditorConsole.hpp"
#include "Core/FileExplorer.hpp"
#include "Core/SceneInspector.hpp"
#include "Core/EntityInspector.hpp"
#include "Core/Viewport.hpp"
#include "Core/Utils/Shortcuts.hpp"

namespace lum::editor {

	using ComponentsEntriesMap = std::unordered_map<HashedString, EditorComponentMetadata>;
	using ComponentsByCategoryMap = std::unordered_map<StringView, std::vector<EditorComponentMetadata*>>;

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

		Window* mWindow;
		rhi::RenderDevice* mRenderDevice;
		render::Renderer* mRenderer;
		Scene* mCurrentScene;

		Console mConsole;
		Viewport mViewport;
		FileExplorer mExplorer;
		SceneInspector mSceneInspector;
		EntityInspector mEntityInspector;

		inline static ComponentsEntriesMap skComponentsEntries;
		inline static ComponentsByCategoryMap skComponentsByCategory;

		void draw_menu_bar( );
		void draw_layout( );

		void init_imgui( Window* window );
		void set_flags_recursive( ImGuiDockNode* node, ImGuiDockNodeFlags flags );
		void begin_imgui( );
		void end_imgui( );

	};

}