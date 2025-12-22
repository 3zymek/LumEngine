#include "include/modules/entity/ecs_manager.hpp"
#include "include/modules/entity/entity.hpp"
#include "include/modules/entity/ecs_common.hpp"
namespace ecs {
	Entity EntityManager::CreateEntity( ) {
		auto entityID = detail::GenerateEntityID::Get();

		LOG_INFO("Created entity");

		return Entity(*this, entityID);
	}
}