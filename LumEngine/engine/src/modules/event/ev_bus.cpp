#include "event/ev_bus.hpp"
#include "event/ev_emitter.hpp"
namespace ev {

	EventEmitter EventBus::CreateEventEmitter() {
		return EventEmitter(*this);
	}

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
		LOG_INIT_OK("Event Bus initialization");
	}
}
