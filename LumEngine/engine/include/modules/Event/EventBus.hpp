//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core event dispatch system for the Lum Engine.
//
//=============================================================================//
#pragma once
#include "Event/EventCommon.hpp"
#include "Event/EventPool.hpp"

namespace lum {
	namespace ev {

		/*! @class EventBus
		*  @brief Core event dispatch system for the engine.
		*
		*  @thread_safety All operations must be called from the main thread
		*        or the thread that owns the EventBus.
		*/
		class EventBus {
		public:

			EventBus( ) { initialize_pools( ); }
			~EventBus( ) { destroy_pools( ); }

			/*! @brief Emits an event to all subscribers of the given type.
			*
			*  Iterates through the pool of the specified EventType and calls all active callbacks.
			*
			*  @tparam EventType Type of event to emit. Must satisfy LumEvent concept.
			*  @param event Reference to the event instance to dispatch.
			*  @thread_safety Call from the main thread or the thread that owns the EventBus.
			*/
			template< detail::cEvent EventType >
			void Emit( const EventType& event ) {
				get_pool<EventType>( ).Emit( event );
			}

			/*! @brief Subscribes a one-time callback to an event type.
			*
			*  The callback will be invoked once when the event is emitted and automatically removed.
			*
			*  @tparam EventType Type of event to subscribe to.
			*  @tparam Lambda Type of the callback lambda/functor.
			*  @param lambda Callback function to invoke on event.
			*  @return SubscriptionID Handle to unsubscribe manually if needed.
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::cEvent EventType, typename Lambda >
			detail::SubscriptionID Subscribe( Lambda&& lambda ) {
				return get_pool<EventType>( ).Subscribe( std::forward<Lambda>( lambda ) );
			}

			/*! @brief Subscribes a permanent callback to an event type.
			*
			*  The callback will persist until manually unsubscribed. Useful for systems that need continuous event notifications.
			*
			*  @tparam EventType Type of event to subscribe to.
			*  @tparam Lambda Type of the callback lambda/functor.
			*  @param lambda Callback function to invoke on event.
			*  @return SubscriptionID Handle for manual unsubscription.
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::cEvent EventType, typename Lambda >
			detail::SubscriptionID SubscribePermanently( Lambda&& lambda ) {
				return get_pool<EventType>( ).SubscribePermanently( std::forward<Lambda>( lambda ) );
			}

			/*! @brief Unsubscribes a previously registered one-time callback.
			*
			*  Removes the callback identified by SubscriptionID from the EventType pool.
			*
			*  @tparam EventType Type of event the callback was subscribed to.
			*  @param id Handle returned by Subscribe().
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::cEvent EventType >
			void Unsubscribe( detail::SubscriptionID id ) {
				get_pool<EventType>( ).Unsubscribe( id );
			}

			/*! @brief Unsubscribes a previously registered permanent callback.
			*
			*  Removes the callback identified by SubscriptionID from the EventType pool.
			*
			*  @tparam EventType Type of event the callback was subscribed to.
			*  @param id Handle returned by SubscribePermanently().
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::cEvent EventType >
			void UnsubscribePermanent( detail::SubscriptionID id ) {
				get_pool<EventType>( ).UnsubscribePermanent( id );
			}

			/*! @brief Polls all event pools and executes pending events.
			*
			*  Iterates over all allocated EventPools and calls PollEvents on each to dispatch queued events.
			*
			*  @thread_safety Call from the main thread or the thread that owns the EventBus.
			*/
			void FlushEvents( ) {
				for (usize i = 0; i < limits::kMaxEventTypes; i++) {
					if (mPools[ i ])
						mPools[ i ]->FlushEvents( );
				}
			}

		private:

			detail::EventPoolBase* mPools[ limits::kMaxEventTypes ]{};

			/* @brief Returns or creates the event pool for the given event type.
			*  Allocates a new EventPool on first access for the given type.
			*  @tparam tType Event type to retrieve the pool for.
			*  @return Reference to the EventPool for tType.
			*/
			template<detail::cEvent tType>
			detail::EventPool<tType>& get_pool( ) {

				auto typeID = TypeRegistry::GetTypeId<tType>( );

				LUM_ASSERT( typeID < limits::kMaxEventTypes, "Max event types reached" );

				auto& ptr = mPools[ typeID ];

				if (!ptr) {
					ptr = new detail::EventPool<tType>( );
				}

				return *static_cast< detail::EventPool<tType>* >( ptr );
			}

			/* @brief Initializes all pool slots to nullptr. */
			void initialize_pools( ) {
				for (usize i = 0; i < limits::kMaxEventTypes; i++) {
					mPools[ i ] = nullptr;
				}
			}

			/* @brief Deletes all allocated event pools and resets slots to nullptr. */
			void destroy_pools( ) {
				for (usize i = 0; i < limits::kMaxEventTypes; i++) {
					if (mPools[ i ] != nullptr) {
						delete mPools[ i ];
						mPools[ i ] = nullptr;
					}
				}
			}

		};

	}
}