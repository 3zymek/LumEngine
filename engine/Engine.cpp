#include "Engine.hpp"
#include "ObjectManager.hpp"
#include "EventManager.hpp"
#include "Object.hpp"
void Engine::init() {

}
void Engine::run() {
	while (true) {
		m_objManager->update1All();
		m_objManager->update2All();
		m_objManager->trashObjects();
	}
}

#ifdef DEBUG_ENGINE

Engine::EM& Engine::getGameEventManager() {
	return *m_GameEventSystem;
}
Engine::OM& Engine::getObjectManager() {
	return *m_objManager;
}

#endif // DEBUG