#include "include/modules/event/ev_bus.hpp"
namespace ev {
	void EventBus::ProcessAll() {
		for (size_t i = 0; i < detail::MAX_EVENT_TYPES; i++) {
			if (m_pools[i]) {
				m_pools[i]->Process();
			}
		}
	}
	void EventBus::Kill() {
		for (size_t i = 0; i < detail::MAX_EVENT_TYPES; i++) {
			if (m_pools[i]) {
				delete m_pools[i];
				m_pools[i] = nullptr;
			}
		}
	}

	void EventBus::Init() {
		for (size_t i = 0; i < detail::MAX_EVENT_TYPES; i++) {
			m_pools[i] = nullptr;
		}
	}
}
