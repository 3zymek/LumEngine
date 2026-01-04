#pragma once
#include "entity/ecs_common.hpp"
#include "entity/ecs_manager.hpp"
#include "entity/components/c_transform.hpp"
#include "entity/entity.hpp"
namespace lum::systems {

	class TransformSystem {

		using TransformComponent = ecs::components::TransformComponent;

	public:

		TransformSystem( ecs::EntityManager& ecs_mgr ) : ecs_manager(ecs_mgr) 
			{ m_dirty_entities.reserve(MAX_DIRTY_TRANSFORMS_PER_FRAME); }

		void SetPosition( const Entity& e, const glm::vec3 position ) {
			auto* transform = ecs_manager.GetComponent<TransformComponent>( e.GetID() );
			HOTPATH_CHECK_RETURN_VOID(!transform, "Entity doesn't have transform component");
			transform->position = position;
			transform->dirty = true;
			if (m_dirty_entities.size() <= MAX_DIRTY_TRANSFORMS_PER_FRAME && !transform->dirty) {
				m_dirty_entities.push_back(e.GetID());
			}
		}

		void Update( ) {
			for (auto& entityID : m_dirty_entities) {
				auto* transform = ecs_manager.GetComponent<TransformComponent>(entityID);
				if (CheckTransform(transform, "Entity doesn't have transform component")) continue;
				CalculateMatrix(transform);
			}
			m_dirty_entities.clear();
		}

	private:

		void CalculateMatrix( TransformComponent* transform ) {
			glm::mat4 new_model = glm::mat4(1.f);
			glm::quat q_rotation = glm::quat(glm::radians(transform->rotation));
			glm::mat4 rotation_matrix = glm::mat4_cast(q_rotation);

			new_model	= glm::scale	( new_model, transform->scale )
						* glm::mat4_cast( glm::quat(glm::radians(transform->rotation)) )
						* glm::translate( new_model, transform->position );
			
			transform->model = new_model;
		}

		inline static bool CheckTransform( auto* transform, const char* msg ) {
			#if defined(HOTPATH_VALIDATION)
				if (!transform) { LOG_WARN(msg); return false; }
			#endif
			return true;
		}

		inline static constexpr size_t MAX_DIRTY_TRANSFORMS_PER_FRAME = 1000;

		std::vector<ecs::EntityID> m_dirty_entities;

		ecs::EntityManager& ecs_manager;

	};

}