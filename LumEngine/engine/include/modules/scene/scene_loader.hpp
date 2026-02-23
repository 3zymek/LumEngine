#pragma once

#include "core/core_common.hpp"

/*
* Create own parser, tokenizer etc for engine format
* .lsc -> scene containing whole configuration
* .lmt -> material
* .lprefab ( if needed )
*/
//=====BACKEND======
//#include <json.hpp>
//==================

namespace lum {

	struct Scene;
	struct FSceneManagerContext;

	class SceneLoader {
	public:
		
		Scene Load( ccharptr path, FSceneManagerContext& ctx );

	};

}