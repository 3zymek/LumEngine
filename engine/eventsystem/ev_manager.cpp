#include "ev_manager.hpp"
namespace ev {
	void EventManager::ProcessAll() {
		for (size_t i = 0; i < detail::MAX_EVENT_TYPES; i++) {
			if (m_pools[i]) {
				m_pools[i]->Process();
			}
		}
	}
	void EventManager::Kill() {
		for (size_t i = 0; i < detail::MAX_EVENT_TYPES; i++) {
			if (m_pools[i]) {
				delete m_pools[i];
				m_pools[i] = nullptr;
			}
		}
	}

	void EventManager::Init() {
		for (size_t i = 0; i < detail::MAX_EVENT_TYPES; i++) {
			m_pools[i] = nullptr;
		}
	}
}
