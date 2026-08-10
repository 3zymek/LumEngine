//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Parser for .lsc scene format files.
//
//=============================================================================//
#pragma once
#include "Scene/Format/FormatCommon.hpp"
#include "Entity/EntityManager.hpp"

namespace lum { struct Scene; }
namespace lum::fmt {

	struct Tokenizer;
	struct Token;

	/* @brief Parses a tokenized .lsc scene file and populates a Scene.
	* Dispatches identifiers and component blocks to their respective parse functions
	* via hash-keyed lookup tables for efficient routing.
	*/
	class SceneDependencyManager {
	public:

		void Initialize( SceneManagerContext& ctx );

		/* @brief Parses the token stream and populates the given scene.
		* @param scene Scene to populate with entities and components.
		*/
		void Deserialize( Scene& scene, Tokenizer& tokenizer );

		void Serialize( Scene& scene );

	private:

		/* @brief Parses the world block and applies global scene settings. */
		static void deserialize_world( std::vector<Token>& tokens, int32& i, ParseContext& ctx );

		/* @brief Parses an entity block and creates a new entity in the scene. */
		static void deserialize_entity( std::vector<Token>& tokens, int32& i, ParseContext& ctx );

		static void categorize_component_infos( );

		/* @brief Resource manager context used during parsing for asset resolution. */
		SceneManagerContext* mCtx = nullptr;

		/* @brief Lookup table mapping hashed identifier keywords to their parse functions.
		* Used to dispatch top-level scene constructs such as entity and world blocks.
		*/
		static inline std::unordered_map<uint64, DeserializeFn> sIdentifiersDeserializeFunctions = {
			{ HashString( "entity" ), deserialize_entity },
			{ HashString( "world" ),  deserialize_world  }
		};

		static inline std::unordered_map<uint64, SceneComponentInfo*> mNameInfoLookup{};
		static inline std::unordered_map<uint64, SceneComponentInfo*> mTypeIdInfoLookup{};

		static inline std::vector<SceneComponentInfo> mComponentsInfos{};

	};

} // namespace lum::fmt