//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Scene management — stores, loads and provides access to active scenes.
//
//=============================================================================//

#include "Core/Utils/ResourceLoader.hpp"

#include "Scene/SceneManager.hpp"
#include "Scene/Format/SceneDepManager.hpp"
#include "Scene/Format/Tokenizer.hpp"

#include "Entity/EntityManager.hpp"

#include "Entity/Components/Transform.hpp"

namespace lum {

//=======================================================//
	// Public
//=======================================================//

	void SceneManager::Initialize( SceneManagerContext& ctx ) {

		m_Ctx = ctx;
		m_SceneDependencyMgr.Initialize( m_Ctx );

	}

	void SceneManager::SetScene( const Path& scenePath ) {

		uint64 hash = scenePath.Hash();

		if (!m_Scenes.contains( hash )) {

			LoadScene( scenePath );

		}

		m_CurrentScene = &m_Scenes[ hash ];
		LUM_LOG_INFO( "Set current scene: {}", scenePath.ToString( ) );

	}

	void SceneManager::LoadScene( const Path& scenePath ) {

		uint64 hash = scenePath.Hash();

		Path fullPath = ResourceLoader::ResolveResourcePath( ResourceRoot::External, scenePath );
		auto content = FileSystem::ReadAllText( fullPath );

		if (!content) {
			LUM_LOG_ERROR( "Failed to load scene {}: {}", scenePath.ToString( ), content.GetError() );
			return;
		}
		if (!fmt::IsValidFormat( scenePath, fmt::FileFormat::Scene )) {
			String str = scenePath.ToString();
			LUM_LOG_ERROR("Invalid scene format: {}", str);
			return;
		}

		fmt::Tokenizer tokenizer;
		tokenizer.Tokenize( content.ValueRef( ), scenePath );

		SceneInstance scene{};
		scene.m_EntityMgr.Initialize( m_Ctx.m_EventBus() );
		scene.m_ScenePath = fullPath;

		m_SceneDependencyMgr.Deserialize( scene, tokenizer );
		m_Scenes.emplace( hash, std::move( scene ) );

		LUM_LOG_INFO( "Loaded scene: {}", scenePath.ToString( ) );

	}

	void SceneManager::SaveScene( SceneInstance& scene ) {

		m_SceneDependencyMgr.Serialize( scene );
		LUM_LOG_INFO( "Saved scene: {}", scene.m_ScenePath.ToString() );
	   
	}

	SceneInstance* SceneManager::GetCurrentScene( ) {
		if (m_CurrentScene != nullptr)
			return m_CurrentScene;
		return nullptr;
	}

}
