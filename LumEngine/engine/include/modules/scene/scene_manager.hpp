#pragma once
#include "core/core_pch.hpp"
#include "entity/ecs_manager.hpp"

namespace lum {

	class MTextureManager;
	class MMaterialManager;
	class MMeshManager;
	class MShaderManager;

	namespace render { class Renderer; }

	struct Scene {
	
		std::vector<ecs::EntityID> mEntities;
		ecs::MEntityManager mEntityMgr;
		
	};

	struct FSceneManagerContext {

		MTextureManager*	mTextureMgr = nullptr;
		MMaterialManager*	mMaterialMgr = nullptr;
		MMeshManager*		mMeshMgr = nullptr;
		MShaderManager*		mShaderMgr = nullptr;
		render::Renderer*	mRenderer = nullptr;

	};

	class MSceneManager {
	public:

		void Initialize( FSceneManagerContext& ctx );

		void SetScene( ccharptr scenePath );

		Scene* GetCurrentScene( );

	private:

		FSceneManagerContext mContext;

		Scene* mCurrentScene = nullptr;
		
		std::unordered_map<uint64, Scene> mScenes;
		
	};

}