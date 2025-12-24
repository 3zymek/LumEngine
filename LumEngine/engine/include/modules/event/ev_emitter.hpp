#pragma once
#include "event/ev_common.hpp"
#include "event/ev_bus.hpp"
#include "core/core_pch.hpp"
namespace ev {

	// Wrapper to secure events subscribtions
	class EventEmitter {
	public:

		EventEmitter(EventBus& em) : m_event(em) {}
		~EventEmitter() { Destruct(); }

		/*! @brief Subscribes a callback to a specific event.
		*
		*  Registers a callback of type `Arg` to the event `T`. The subscription
		*  is automatically tracked and will be unsubscribed when this object is destroyed.
		*
		*  @tparam T The type of the event to subscribe to.
		*  @tparam Arg Type of the callback (function, lambda, functor).
		*  @param arg The callback to register.
		*  @return CallbackID A unique identifier for this subscription.
		*
		*  @thread_safety Call from the main thread or the event system context.
		*/
		template<detail::EventT T, typename Arg>
		inline detail::CallbackID Subscribe(Arg&& arg) {
			auto id = m_event.Subscribe<T>(std::forward<Arg>(arg));
			m_destructors.push_back([id, this]() { this->m_event.Unsubscribe<T>(id); });
			return id;
		}

		/*! @brief Subscribes a permanent callback to a specific event.
		*
		*  Registers a callback of type `Arg` to the event `T`. Unlike regular Subscribe,
		*  this subscription is considered permanent, but still tracked for automatic
		*  unsubscription when this object is destroyed.
		*
		*  @tparam T The type of the event to subscribe to.
		*  @tparam Arg Type of the callback (function, lambda, functor).
		*  @param arg The callback to register.
		*  @return CallbackID A unique identifier for this subscription.
		*
		*  @thread_safety Call from the main thread or the event system context.
		*/
		template<detail::EventT T, typename Arg>
		inline detail::CallbackID SubscribePermamently(Arg&& arg) {
			auto id = m_event.SubscribePermamently<T>(std::forward<Arg>(arg));
			m_destructors.push_back([id, this]() { this->m_event.Unsubscribe<T>(id); });
			return id;
		}

		/*! @brief Unsubscribes a callback from a specific event.
		*
		*  Removes the callback identified by `id` from the event `T`.
		*
		*  @tparam T The type of the event to unsubscribe from.
		*  @param id The CallbackID obtained when subscribing.
		*
		*  @thread_safety Call from the main thread or the event system context.
		*/
		template<detail::EventT T>
		inline void Unsubscribe(const detail::CallbackID& id) {
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