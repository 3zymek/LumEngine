/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once

#include <vector>
#include <unordered_map>

#include "Core/Utils/StringUtils.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Scene/Format/FormatTypes.hpp"
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

		void Initialize(SceneManagerContext& ctx);

		/* @brief Parses the token stream and populates the given scene.
		* @param scene Scene to populate with entities and components.
		*/
		void Deserialize(Scene& scene, Tokenizer& tokenizer);

		void Serialize(Scene& scene);

	private:

		/* @brief Parses the world block and applies global scene settings. */
		static void deserialize_world(std::vector<Token>& tokens, int32& i, DeserializeContext& ctx);

		/* @brief Parses an entity block and creates a new entity in the scene. */
		static void deserialize_entity(std::vector<Token>& tokens, int32& i, DeserializeContext& ctx);

		static void read_children(std::vector<Token>& tokens, int32& i, DeserializeContext& ctx);
		static void write_children(StringBuilder& sb, Scene& scene, EntityID entity);

		static void categorize_component_infos();

		/* @brief Resource manager context used during parsing for asset resolution. */
		SafePtr<SceneManagerContext> m_Ctx = nullptr;

		/* @brief Lookup table mapping hashed identifier keywords to their parse functions.
		* Used to dispatch top-level scene constructs such as entity and world blocks.
		*/
		static inline std::unordered_map<HashedString, DeserializeFn> s_IdentifiersDeserializeFunctions = {
			{ HashString("entity"), deserialize_entity },
			{ HashString("world"),  deserialize_world }
		};

		static inline std::unordered_map<HashedString, DeserializeFn> s_EntityProperyHandlers = {
			{ HashString("children"), read_children }
		};

		static inline std::unordered_map<HashedString, SceneComponentInfo*> s_NameInfoLookup{};
		static inline std::unordered_map<HashedString, SceneComponentInfo*> s_TypeIdInfoLookup{};
		static inline std::vector<SceneComponentInfo> s_ComponentsInfos{};

	};

} // namespace lum::fmt