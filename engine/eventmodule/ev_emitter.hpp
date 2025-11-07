#pragma once
#include "ev_define.hpp"
#include "ev_bus.hpp"
#include <vector>
namespace ev {

	class EventEmitter {
	public:

		EventEmitter(EventBus& em) : m_event(em) {}
		~EventEmitter() { Destruct(); }

		template<detail::EventT T, typename Arg>
		force_inline detail::CallbackID Subscribe(Arg&& arg) {
			auto id = m_event.Subscribe<T>(std::forward<Arg>(arg));
			m_destructors.push_back([id, this]() { this->m_event.Unsubscribe<T>(id); });
			return id;
		}

		template<detail::EventT T, typename Arg>
		force_inline detail::CallbackID SubscribePermamently(Arg&& arg) {
			auto id = m_event.SubscribePermamently<T>(std::forward<Arg>(arg));
			m_destructors.push_back([id, this]() { this->m_event.Unsubscribe<T>(id); });
			return id;
		}

		template<detail::EventT T>
		force_inline void Unsubscribe(const detail::CallbackID& id) {
			m_event.Unsubscribe<T>(id);
		}

	private:

		EventBus& m_event;
		std::vector<detail::DestructFunction> m_destructors;

		void Destruct() {
			for (auto& f : m_destructors) {
				f();
			}
			m_destructors.clear();
		}

	};

}