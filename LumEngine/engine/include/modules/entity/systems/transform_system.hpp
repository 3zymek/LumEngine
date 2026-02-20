#pragma once
#include "entity/ecs_common.hpp"
#include "entity/ecs_manager.hpp"
#include "entity/components/c_transform.hpp"
#include "entity/entity.hpp"
namespace lum::systems {

	class TransformSystem {

		using TransformComponent = CTransform;

	public:

		TransformSystem( ecs::MEntityManager& ecs_mgr ) : ecs_manager(ecs_mgr) 
			{ m_dirty_entities.reserve(MAX_DIRTY_TRANSFORMS_PER_FRAME); }

		void SetPosition( const Entity& e, const glm::vec3 position ) {
			auto* transform = ecs_manager.GetComponent<TransformComponent>( e.GetID() );
			LUM_HOTCHK_RETURN_VOID(transform, LogSeverity::Warn, "Entity doesn't have transform component");
			transform->position = position;
			if (m_dirty_entities.size() < MAX_DIRTY_TRANSFORMS_PER_FRAME && !transform->bDirty) {
				m_dirty_entities.push_back(e.GetID());
				transform->bDirty = true;
			}
		}

		void SetRotation(const Entity& e, const glm::vec3 rotation) {
			auto* transform = ecs_manager.GetComponent<TransformComponent>(e.GetID());
			LUM_HOTCHK_RETURN_VOID(transform, LUM_SEV_WARN, "Entity doesn't have transform component");
			transform->rotation = rotation;
			if (m_dirty_entities.size() < MAX_DIRTY_TRANSFORMS_PER_FRAME && !transform->bDirty) {
				m_dirty_entities.push_back(e.GetID());
				transform->bDirty = true;
			}
		}

		void SetScale(const Entity& e, const glm::vec3 scale) {
			auto* transform = ecs_manager.GetComponent<TransformComponent>(e.GetID());
			LUM_HOTCHK_RETURN_VOID(transform, LUM_SEV_WARN, "Entity doesn't have transform component");
			transform->scale = scale;
			if (m_dirty_entities.size() < MAX_DIRTY_TRANSFORMS_PER_FRAME && !transform->bDirty) {
				m_dirty_entities.push_back(e.GetID());
				transform->bDirty = true;
			}
		}

		void Update( ) {

			int size = m_dirty_entities.size();

			for (auto& entityID : m_dirty_entities) {
				auto* transform = ecs_manager.GetComponent<TransformComponent>(entityID);
				if (!check_transform(transform, "Entity doesn't have transform component")) continue;
				calculate_matrix(transform);
				transform->bDirty = false;
			}
			if(size > 0) m_dirty_entities.clear();
		}

	private:

		void calculate_matrix( TransformComponent* transform ) {
			glm::mat4 new_model = glm::mat4(1.f);
			new_model	= glm::translate( new_model, transform->position )
						* glm::mat4_cast( glm::quat(glm::radians(transform->rotation)) )
						* glm::scale	( new_model, transform->scale );
			
		}

		inline static bool check_transform( auto* transform, const char* msg ) {
			#if defined(LUM_HOTPATH_VALIDATION)
				if (!transform) { LUM_LOG_WARN(msg); return false; }
			#endif
			return true;
		}

		inline static constexpr size_t MAX_DIRTY_TRANSFORMS_PER_FRAME = 1000;

		std::vector<ecs::EntityID> m_dirty_entities;

		ecs::MEntityManager& ecs_manager;

	};

}