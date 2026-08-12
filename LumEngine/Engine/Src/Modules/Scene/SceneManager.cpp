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

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void SceneManager::Initialize( SceneManagerContext& ctx ) {

		mCtx = ctx;
		mSceneDependencyMgr.Initialize( mCtx );

	}

	void SceneManager::SetScene( const Path& scenePath ) {

		uint64 hash = scenePath.Hash();

		if (!mScenes.contains( hash )) {

			LoadScene( scenePath );

		}

		mCurrentScene = &mScenes[ hash ];
		LUM_LOG_INFO( "Set current scene: %s", scenePath.ToString( ) );

	}

	void SceneManager::LoadScene( const Path& scenePath ) {

		uint64 hash = scenePath.Hash();

		Path fullPath = ResourceLoader::ResolveResourcePath( ResourceRoot::External, scenePath );
		auto content = FileSystem::ReadAllText( fullPath );

		if (!content) {
			LUM_LOG_ERROR( "Failed to load scene %s: %s", scenePath.ToString( ), content.GetError() );
			return;
		}
		if (!fmt::IsValidFormat( scenePath, fmt::FileFormat::Scene )) {
			String str = scenePath.ToString();
			LUM_LOG_ERROR("Invalid scene format: %s", str);
			return;
		}

		fmt::Tokenizer tokenizer;
		tokenizer.Tokenize( content.ValueRef( ), scenePath );

		Scene scene{};
		scene.mEntityMgr.Initialize( mCtx.mEventBus );
		scene.mScenePath = fullPath;

		mSceneDependencyMgr.Deserialize( scene, tokenizer );
		mScenes.emplace( hash, std::move( scene ) );

		LUM_LOG_INFO( "Loaded scene: %s", scenePath.ToString( ) );

	}

	void SceneManager::SaveScene( Scene& scene ) {

		mSceneDependencyMgr.Serialize( scene );
		LUM_LOG_INFO( "Saved scene: %s", scene.mScenePath.ToString() );
	   
	}

	Scene* SceneManager::GetCurrentScene( ) {
		if (mCurrentScene != nullptr)
			return mCurrentScene;
		LUM_LOG_ERROR( "No scene set" );
		return nullptr;
	}

}
