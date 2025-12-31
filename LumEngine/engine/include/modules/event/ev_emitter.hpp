#pragma once
#include "core/core_pch.hpp"
#include "modules/event/ev_common.hpp"
#include "event/ev_bus.hpp"
namespace ev {

	class EventEmitter {
	public:

		EventEmitter(EventBus& bus) : m_bus(bus) {}

		template<detail::LumEvent EventType>
		void Emit(const EventType& event) {
			m_bus.Emit<EventType>(event);
		}

		template<detail::LumEvent EventType, typename Lambda>
		void Subscribe(Lambda&& lambda) {
			m_bus.Subscribe<EventType>(std::forward<Lambda>(lambda));
		}

		template<detail::LumEvent EventType, typename Lambda>
		void SubscribePermamently(Lambda&& lambda) {
			m_bus.SubscribePermamently<EventType>(std::forward<Lambda>(lambda));
		}

	private:

		EventBus& m_bus;

		detail::SubscribtionID m_subscribed_id[detail::MAX_PERM_CALLBACKS];

	};


}