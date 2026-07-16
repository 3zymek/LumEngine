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

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void SceneManager::Initialize( SceneManagerContext& ctx ) {

		mContext = ctx;

	}

	void SceneManager::SetScene( StringView scenePath ) {

		uint64 hash = HashString( scenePath );

		if (!mScenes.contains( hash )) {

			LoadScene( scenePath );

		}

		mCurrentScene = &mScenes[ hash ];

	}

	void SceneManager::LoadScene( StringView scenePath ) {

		uint64 hash = HashString( scenePath );

		std::optional<String> content = ResourceLoader::ReadTextFile( ResourceRoot::External, scenePath );

		if (!content) {
			LUM_LOG_ERROR( "Failed to load scene %s: %s", scenePath, ResourceLoader::GetErrorMessage( ) );
			return;
		}
		if (!fmt::IsValidFormat( scenePath, fmt::Format::Scene )) {
			LUM_LOG_ERROR( "Invalid scene format: %s", scenePath );
			return;
		}

		fmt::Tokenizer tokenizer;
		tokenizer.Tokenize( content.value( ) );

		fmt::SceneDependencyManager parser;
		parser.Initialize( tokenizer, mContext );
		Scene scene;
		scene.mEntityMgr.Initialize( mContext.mEventBus );
		parser.Parse( scene );
		mScenes.emplace( hash, std::move( scene ) );

	}

	void SaveScene( StringView scenePath ) {

		

	}

	Scene* SceneManager::GetCurrentScene( ) {
		if (mCurrentScene != nullptr)
			return mCurrentScene;
		LUM_LOG_ERROR( "No scene set" );
		return nullptr;
	}

}