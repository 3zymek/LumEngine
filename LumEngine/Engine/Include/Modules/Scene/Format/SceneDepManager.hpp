//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Parser for .lsc scene format files.
//
//=============================================================================//
#pragma once
#include "Scene/Format/FormatCommon.hpp"
#include "Entity/EntityManager.hpp"

namespace lum { struct SceneInstance; }
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
		void Deserialize( SceneInstance& scene, Tokenizer& tokenizer );

		void Serialize( SceneInstance& scene );

	private:

		/* @brief Parses the world block and applies global scene settings. */
		static void deserialize_world( std::vector<Token>& tokens, int32& i, DeserializeContext& ctx );

		/* @brief Parses an entity block and creates a new entity in the scene. */
		static void deserialize_entity( std::vector<Token>& tokens, int32& i, DeserializeContext& ctx );

		static void read_children( std::vector<Token>& tokens, int32& i, DeserializeContext& ctx );
		static void write_children( StringBuilder& sb, SceneInstance& scene, EntityID entity );

		static void categorize_component_infos( );

		/* @brief Resource manager context used during parsing for asset resolution. */
		SceneManagerContext* m_Ctx = nullptr;

		/* @brief Lookup table mapping hashed identifier keywords to their parse functions.
		* Used to dispatch top-level scene constructs such as entity and world blocks.
		*/
		static inline std::unordered_map<HashedString, DeserializeFn> sIdentifiersDeserializeFunctions = {
			{ HashString( "entity" ), deserialize_entity },
			{ HashString( "world" ),  deserialize_world  }
		};

		static inline std::unordered_map<HashedString, DeserializeFn> sEntityProperyHandlers = {
			{ HashString( "children" ), read_children }
		};

		static inline std::unordered_map<HashedString, SceneComponentInfo*> sNameInfoLookup{};
		static inline std::unordered_map<HashedString, SceneComponentInfo*> sTypeIdInfoLookup{};
		static inline std::vector<SceneComponentInfo> m_ComponentsInfos{};

	};

} // namespace lum::fmt