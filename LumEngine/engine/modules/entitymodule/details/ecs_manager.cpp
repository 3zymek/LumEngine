#include "details/ecs_manager.hpp"
#include "entity.hpp"
#include "details/ecs_define.hpp"
namespace ecs {
	Entity EntityManager::CreateEntity( ) {
		auto entityID = detail::GenerateEntityID::Get();
		return Entity(*this, entityID);
	}
}