#pragma once
#include "core/core_pch.hpp"
#include "event/ev_common.hpp"
#include "event/ev_pool.hpp"
namespace ev {
	
	class EventBus {
	public:

		EventBus() { Init(); }

		template<detail::LumEvent EventType>
		void Emit(const EventType& event) {
			GetOrCreatePool<EventType>().Emit(event);
		}

		template<detail::LumEvent EventType, typename Lambda>
		void Subscribe(Lambda&& lambda) {
			GetOrCreatePool<EventType>().Subscribe(std::forward<Lambda>(lambda));
		}

		template<detail::LumEvent EventType, typename Lambda>
		detail::SubscribtionHandle SubscribePermamently(Lambda&& lambda) {
			return GetOrCreatePool<EventType>().SubscribePermamently(std::forward<Lambda>(lambda));
		}

	private:
		
		template<detail::LumEvent EventType>
		detail::EventPool<EventType>& GetOrCreatePool() {
			auto typeID = detail::GetEventTypeID::Get<EventType>();
			if (!m_pools[typeID]) {
				m_pools[typeID] = new detail::EventPool<EventType>();
			}
			return *static_cast<detail::EventPool<EventType>*>(m_pools[typeID]);
		}

		void Init() {
			for (size_t i = 0; i < detail::MAX_EVENT_TYPES; i++) {
				m_pools[i] = nullptr;
			}
		}

		detail::BasePool* m_pools[detail::MAX_EVENT_TYPES];

	};

}
#include "ev_bus.ipp"
