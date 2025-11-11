#pragma once
#include "eventmodule/details/ev_bus.hpp"
namespace ev {
	template<detail::EventT T>
	void EventBus::RegisterPool() {
		auto typeID = detail::EventTypeID::Get<T>();
		if (!m_pools[typeID]) {
			m_pools[typeID] = new detail::EventPool<T>();
		}
	}

	template<detail::EventT T, typename Arg>
	detail::CallbackID EventBus::Subscribe(Arg&& arg) {
		auto call = detail::CallbackWrapper<T>(std::forward<Arg>(arg));
		auto id = call.GetID();
		GetPool<T>().SubscribeCallback(std::move(call));
		EVENT_LOG("Subscribed " << typeid(T).name());
		return id;
	}

	template<detail::EventT T, typename Arg>
	detail::CallbackID EventBus::SubscribePermamently(Arg&& arg) {
		auto call = detail::CallbackWrapper<T>(std::forward<Arg>(arg));
		auto id = call.GetID();
		GetPool<T>().SubscribePermamently(std::move(call));
		EVENT_LOG("Subscribed Permamently " << typeid(T).name());
		return id;
	}

	template<detail::EventT T>
	void EventBus::Unsubscribe(const detail::CallbackID& id) {
		GetPool<T>().Unsubscribe(id);
	}

	template<detail::EventT T>
	void EventBus::Emit(const T& event) {
		GetPool<T>().PushEvent(std::move(event));
		EVENT_LOG("Emitting event: " << typeid(T).name());
	}

	template<detail::EventT T>
	detail::EventPool<T>& EventBus::GetPool() {
		auto id = detail::EventTypeID::Get<T>();
		if (!m_pools[id]) {
			RegisterPool<T>();
		}
		return *static_cast<detail::EventPool<T>*>(m_pools[id]);
	}

}