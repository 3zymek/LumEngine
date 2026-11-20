//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Parser for .lsc scene format files.
//
//=============================================================================//
#pragma once

#include "Scene/Format/SceneDepManager.hpp"
#include "Scene/Format/Tokenizer.hpp"
#include "Scene/Format/DeserializeException.hpp"
#include "Scene/SceneManager.hpp"
#include "Core/Utils/StringBuilder.hpp"
#include "Scene/Format/DeserializeFunctions.gen.hpp"

namespace lum::fmt {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void SceneDependencyManager::Initialize(SceneManagerContext& ctx) {

		m_Ctx = &ctx;

		RegisterSceneComponents(s_ComponentsInfos);
		categorize_component_infos();

	}


	void SceneDependencyManager::Deserialize(Scene& scene, Tokenizer& tokenizer) {

		DeserializeContext ctx{
			.m_Scene = scene,
			.m_Ctx = m_Ctx.Ref()
		};

		auto tokens = tokenizer.GetTokens();

		try {
			for (int32 i = 0; i < tokens.size(); i++) {
				if (tokens[i].m_Type == TokenType::Identifier) {
					auto it = s_IdentifiersDeserializeFunctions.find(HashString(ToLower(tokens[i].m_Value)));
					if (it != s_IdentifiersDeserializeFunctions.end()) {
						it->second(tokens, i, ctx);
					}
				}
			}
		}
		catch (const fmt::DeserializeException& e) {
			LUM_LOG_ERROR(e.what());
		}

		for (auto& [entityId, children] : ctx.m_PersistentChildren) {

			for (EntityID child : children) {

				scene.AttachChild(
					entityId,
					ctx.m_PersistentToEntity[child]
				);

			}

		}

	}

	void SceneDependencyManager::Serialize(Scene& scene) {

		StringBuilder sb{};

		for (auto& [entityId, entity] : scene.m_Entities) {

			sb.Append("entity ");
			sb.Append(entityId);
			sb.AppendLine(" {");

			write_children(sb, scene, entityId);

			scene.m_EntityMgr.ForEachComponent(
				entityId,
				[&](ComponentBase* comp, ecs::ComponentBasePool* pool) {
					auto it = s_TypeIdInfoLookup.find(pool->GetTypeId());
					if (it != s_TypeIdInfoLookup.end() && (it->second && it->second->m_SerializeFn)) {
						it->second->m_SerializeFn(sb, comp);
					}
				});

			sb.AppendLine("}");

		}

		FileSystem::WriteAllText(scene.m_ScenePath, sb.ToString());

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void SceneDependencyManager::deserialize_world(std::vector<Token>& tokens, int32& i, DeserializeContext& ctx) {

		detail::ExpectOpeningBracketNext(tokens, i);

		while (detail::InBlock(tokens, i)) {

			if (tokens[i].m_Type == TokenType::Component) {

				detail::ExpectOpeningBracketNext(tokens, i);

				while (detail::InBlock(tokens, i)) {

					if (tokens[i].m_Type == TokenType::Parameter) {

						if (detail::IsString(tokens, i, "path")) {
							detail::ExceptColonNext(tokens, i);
							//ctx.m_Ctx().m_Renderer().SetEnvironmentTexture(
								//ctx.m_Ctx().m_TextureMgr().LoadEquirectangularCubemap(tokens[i].m_Value.c_str())
							//);
						}

					}

					i++;

				}
			}

			i++;

		}
	}



	void SceneDependencyManager::deserialize_entity(std::vector<Token>& tokens, int32& i, DeserializeContext& ctx) {

		Entity entity = ctx.m_Scene.CreateEntity();
		EntityID id = entity.GetId();
		ctx.m_CurrentEntity = id;

		++i;
		EntityID persistentId = detail::ReadInt(tokens, i);
		ctx.m_PersistentToEntity.emplace(persistentId, id);

		detail::ExpectOpeningBracketNext(tokens, i);

		while (detail::InBlock(tokens, i)) {

			if (tokens[i].m_Type == TokenType::Component) {

				HashedString hashed = HashString(ToLower(tokens[i].m_Value));

				auto it = s_NameInfoLookup.find(hashed);

				if (it != s_NameInfoLookup.end()) {
					it->second->m_DeserializeFn(tokens, i, ctx);
				}
				else {
					auto special = s_EntityProperyHandlers.find(hashed);

					if (special != s_EntityProperyHandlers.end()) {
						special->second(tokens, i, ctx);
					}
					else {
						throw DeserializeException(
							"Unknown property at line %llu in file %s",
							tokens[i].m_Line,
							tokens[i].m_FilePath.ToString().c_str()
						);
					}
				}
			}

			++i;

		}

	}

	void SceneDependencyManager::read_children(std::vector<Token>& tokens, int32& i, DeserializeContext& ctx) {

		detail::ExceptColonNext(tokens, i);
		detail::ExceptOpeningSquareBracketInPlace(tokens, i);
		i++;

		std::vector<uint64> children{};

		while (detail::InSquareBlock(tokens, i)) {

			children.push_back(detail::ReadInt(tokens, i));
			++i;

		}

		ctx.m_PersistentChildren.emplace(
			ctx.m_CurrentEntity,
			std::move(children)
		);

	}

	void SceneDependencyManager::write_children(StringBuilder& sb, Scene& scene, EntityID entity) {

		auto it = scene.m_Children.find(entity);
		if (it == scene.m_Children.end())
			return;

		sb.Append("\t@children: [ ");

		for (int32 i = 0; i < it->second.size(); i++) {

			sb.Append(it->second[i]);

			if (i + 1 < it->second.size())
				sb.Append(", ");

		}

		sb.AppendLine(" ]");

	}

	void SceneDependencyManager::categorize_component_infos() {

		for (auto& info : s_ComponentsInfos) {
			s_NameInfoLookup.emplace(HashString(info.m_SerializationName), &info);
			s_TypeIdInfoLookup.emplace(info.m_TypeId, &info);
		}

	}

} // namespace lum::fmt
