//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Parser for .lsc scene format files.
//
//=============================================================================//
#pragma once

#include "scene/format/common.hpp"
#include "entity/entity_manager.hpp"
#include "scene/scene_manager.hpp"
#include <imgui.h>

namespace lum {

	struct FScene;

}
namespace lum::fmt {

	struct Tokenizer;
	struct FToken;

	/* @brief Context passed through the scene parsing pipeline.
	* Holds references to the active scene, current entity and all resource managers.
	*/
	struct FParseContext {

		/* @brief Reference to the scene being populated. */
		FScene& mScene;

		/* @brief Entity currently being parsed and populated with components. */
		EntityID mEntity;

		/* @brief Resource manager context used to load and resolve assets. */
		FSceneManagerContext mContext;

	};

	/* @brief Function pointer type for parse dispatch functions. */
	using ParseFn = void(*)(std::vector<FToken>&, int32&, FParseContext&);

	/* @brief Parses a tokenized .lsc scene file and populates a FScene.
	* Dispatches identifiers and component blocks to their respective parse functions
	* via hash-keyed lookup tables for efficient routing.
	*/
	class SceneDependencyManager {
	public:

		void Initialize( Tokenizer& tokenizer, FSceneManagerContext& ctx );

		/* @brief Parses the token stream and populates the given scene.
		* @param scene Scene to populate with entities and components.
		*/
		void Parse( FScene& scene );

	private:

		/* @brief Parses the world block and applies global scene settings. */
		static void parse_world( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses an entity block and creates a new entity in the scene. */
		static void parse_entity( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );

		/* @brief Parses a audio emitter component and attaches it to the current entity. */
		static void parse_audio_emitter( std::vector<FToken>& tokens, int32& i, FParseContext& ctx );


		/* @brief Reference to the tokenizer holding the pre-tokenized scene file. */
		Tokenizer* mTokenizer = nullptr;

		/* @brief Resource manager context used during parsing for asset resolution. */
		FSceneManagerContext* mContext = nullptr;

		/* @brief Lookup table mapping hashed identifier keywords to their parse functions.
		* Used to dispatch top-level scene constructs such as entity and world blocks.
		*/
		static inline std::unordered_map<uint64, ParseFn> sIdentifiersParseFunctions = {
			{ HashStr( "entity" ), parse_entity },
			{ HashStr( "world" ),  parse_world  }
		};

		/* @brief Lookup table mapping hashed component keywords to their parse functions.
		* Used to dispatch component blocks within an entity definition.
		*/
		static inline std::unordered_map<uint64, ParseFn> sComponentsParseFunctions;

	};

} // namespace lum::fmt