#include "entity/ecs_manager.hpp"
#include "entity/entity.hpp"
#include "entity/ecs_common.hpp"
namespace lum {
	namespace ecs {
		Entity EntityManager::CreateEntity() {
			auto entityID = detail::GenerateEntityID::Get();

			LOG_INFO("Created entity");

			return Entity(*this, entityID);
		}
	}
}