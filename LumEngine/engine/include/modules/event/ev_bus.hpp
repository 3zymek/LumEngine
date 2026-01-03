#pragma once
#include "core/core_pch.hpp"
#include "event/ev_common.hpp"
#include "event/ev_pool.hpp"
namespace lum {
	namespace ev {

		/*! @class EventBus
		*  @brief Core event dispatch system for the engine.
		*
		*  EventBus manages all event types in the engine:
		*  - supports both one-time and permanent callbacks,
		*  - zero heap allocations in hot paths,
		*  - modular template-based pools per event type (EventPool<EventType>),
		*  - allows emit, subscribe, and unsubscribe operations in a safe and fast way.
		*
		*  @note Thread-safety: all operations must be called from the main thread
		*        or the thread that owns the EventBus.
		*/
		class EventBus {
		public:

			EventBus( ) { Init(); }

			/*! @brief Emits an event to all subscribers of the given type.
			*
			*  Iterates through the pool of the specified EventType and calls all active callbacks.
			*
			*  @tparam EventType Type of event to emit. Must satisfy LumEvent concept.
			*  @param event Reference to the event instance to dispatch.
			*  @thread_safety Call from the main thread or the thread that owns the EventBus.
			*/
			template< detail::LumEvent EventType >
			void Emit( const EventType& event ) {
				GetOrCreatePool<EventType>().Emit(event);
			}

			/*! @brief Subscribes a one-time callback to an event type.
			*
			*  The callback will be invoked once when the event is emitted and automatically removed.
			*
			*  @tparam EventType Type of event to subscribe to.
			*  @tparam Lambda Type of the callback lambda/functor.
			*  @param lambda Callback function to invoke on event.
			*  @return SubscribtionID Handle to unsubscribe manually if needed.
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::LumEvent EventType, typename Lambda >
			detail::SubscribtionID Subscribe( Lambda&& lambda ) {
				return GetOrCreatePool<EventType>().Subscribe(std::forward<Lambda>(lambda));
			}

			/*! @brief Subscribes a permanent callback to an event type.
			*
			*  The callback will persist until manually unsubscribed. Useful for systems that need continuous event notifications.
			*
			*  @tparam EventType Type of event to subscribe to.
			*  @tparam Lambda Type of the callback lambda/functor.
			*  @param lambda Callback function to invoke on event.
			*  @return SubscribtionID Handle for manual unsubscription.
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::LumEvent EventType, typename Lambda >
			detail::SubscribtionID SubscribePermanently( Lambda&& lambda ) {
				return GetOrCreatePool<EventType>().SubscribePermanently(std::forward<Lambda>(lambda));
			}

			/*! @brief Unsubscribes a previously registered one-time callback.
			*
			*  Removes the callback identified by SubscribtionID from the EventType pool.
			*
			*  @tparam EventType Type of event the callback was subscribed to.
			*  @param id Handle returned by Subscribe().
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::LumEvent EventType >
			void Unsubscribe( detail::SubscribtionID id ) {
				GetOrCreatePool<EventType>().Unsubscribe(id);
			}

			/*! @brief Unsubscribes a previously registered permanent callback.
			*
			*  Removes the callback identified by SubscribtionID from the EventType pool.
			*
			*  @tparam EventType Type of event the callback was subscribed to.
			*  @param id Handle returned by SubscribePermanently().
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::LumEvent EventType >
			void UnsubscribePermanent( detail::SubscribtionID id ) {
				GetOrCreatePool<EventType>().UnsubscribePermanent(id);
			}

			/*! @brief Polls all event pools and executes pending events.
			*
			*  Iterates over all allocated EventPools and calls PollEvents on each to dispatch queued events.
			*
			*  @thread_safety Call from the main thread or the thread that owns the EventBus.
			*/
			void PollEvents( ) {
				for (size_t i = 0; i < detail::MAX_EVENT_TYPES; i++) {
					if (m_pools[i])
						m_pools[i]->PollEvents();
				}
			}



		private:

			template< detail::LumEvent EventType >
			detail::EventPool<EventType>& GetOrCreatePool( ) {
				auto typeID = detail::GetEventTypeID::Get<EventType>();
				if (!m_pools[typeID]) {
					m_pools[typeID] = new detail::EventPool<EventType>();
				}
				return *static_cast<detail::EventPool<EventType>*>(m_pools[typeID]);
			}

			void Init( ) {
				for (size_t i = 0; i < detail::MAX_EVENT_TYPES; i++) {
					m_pools[i] = nullptr;
				}
			}

			detail::BasePool* m_pools[ detail::MAX_EVENT_TYPES ];

		};

	}
}