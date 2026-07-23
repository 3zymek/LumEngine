//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Parser for .lsc scene format files.
//
//=============================================================================//
#pragma once
#include "Scene/Format/FormatCommon.hpp"
#include "Entity/EntityManager.hpp"
#include "Scene/SceneManager.hpp"

namespace lum { struct Scene; }
namespace lum::fmt {

	struct Tokenizer;
	struct FToken;

	/* @brief Parses a tokenized .lsc scene file and populates a Scene.
	* Dispatches identifiers and component blocks to their respective parse functions
	* via hash-keyed lookup tables for efficient routing.
	*/
	class SceneDependencyManager {
	public:

		void Initialize( Tokenizer& tokenizer, SceneManagerContext& ctx );

		/* @brief Parses the token stream and populates the given scene.
		* @param scene Scene to populate with entities and components.
		*/
		void Parse( Scene& scene );

		void Serialize( Scene& scene, StringView path );

	private:

		/* @brief Parses the world block and applies global scene settings. */
		static void parse_world( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses an entity block and creates a new entity in the scene. */
		static void parse_entity( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Reference to the tokenizer holding the pre-tokenized scene file. */
		Tokenizer* mTokenizer = nullptr;

		/* @brief Resource manager context used during parsing for asset resolution. */
		SceneManagerContext* mContext = nullptr;

		/* @brief Lookup table mapping hashed identifier keywords to their parse functions.
		* Used to dispatch top-level scene constructs such as entity and world blocks.
		*/
		static inline std::unordered_map<uint64, ParseFn> sIdentifiersParseFunctions = {
			{ HashString( "entity" ), parse_entity },
			{ HashString( "world" ),  parse_world  }
		};

		/* @brief Lookup table mapping hashed component keywords to their parse functions.
		* Used to dispatch component blocks within an entity definition.
		*/
		static inline std::unordered_map<uint64, SceneComponentInfo> sComponentsInfos;

	};

} // namespace lum::fmt