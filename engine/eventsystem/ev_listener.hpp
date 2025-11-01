#pragma once
#include "ev_define.hpp"
#include "ev_manager.hpp"
#include <vector>
namespace ev {

	class EventListener {
	public:

		EventListener(EventManager& em) : m_event(em) {}
		~EventListener() { Destruct(); }

		template<detail::EventT T, typename Arg>
		FORCEINLINE detail::CallbackID Subscribe(Arg&& arg) {
			auto id = m_event.Subscribe<T>(std::forward<Arg>(arg));
			m_destructors.push_back([id, this]() { this->m_event.Unsubscribe<T>(id); });
			return id;
		}

		template<detail::EventT T, typename Arg>
		FORCEINLINE detail::CallbackID SubscribePermamently(Arg&& arg) {
			auto id = m_event.SubscribePermamently<T>(std::forward<Arg>(arg));
			m_destructors.push_back([id, this]() { this->m_event.Unsubscribe<T>(id); });
			return id;
		}

		template<detail::EventT T>
		FORCEINLINE void Unsubscribe(const detail::CallbackID& id) {
			m_event.Unsubscribe<T>(id);
		}

	private:

		EventManager& m_event;
		std::vector<detail::DestructFunction> m_destructors;

		void Destruct() {
			for (auto& f : m_destructors) {
				f();
			}
			m_destructors.clear();
		}

	};

}