//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core event dispatch system for the Lum Engine.
//
//=============================================================================//
#pragma once
#include "event/event_common.hpp"
#include "event/event_pool.hpp"

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

			EventBus( ) { init( ); }
			~EventBus( ) { destruct( ); }

			/*! @brief Emits an event to all subscribers of the given type.
			*
			*  Iterates through the pool of the specified EventType and calls all active callbacks.
			*
			*  @tparam EventType Type of event to emit. Must satisfy LumEvent concept.
			*  @param event Reference to the event instance to dispatch.
			*  @thread_safety Call from the main thread or the thread that owns the EventBus.
			*/
			template< detail::tEvent EventType >
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
			*  @return SubscribtionID Handle to unsubscribe manually if needed.
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::tEvent EventType, typename Lambda >
			detail::SubscribtionID Subscribe( Lambda&& lambda ) {
				return get_pool<EventType>( ).Subscribe( std::forward<Lambda>( lambda ) );
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
			template< detail::tEvent EventType, typename Lambda >
			detail::SubscribtionID SubscribePermanently( Lambda&& lambda ) {
				return get_pool<EventType>( ).SubscribePermanently( std::forward<Lambda>( lambda ) );
			}

			/*! @brief Unsubscribes a previously registered one-time callback.
			*
			*  Removes the callback identified by SubscribtionID from the EventType pool.
			*
			*  @tparam EventType Type of event the callback was subscribed to.
			*  @param id Handle returned by Subscribe().
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::tEvent EventType >
			void Unsubscribe( detail::SubscribtionID id ) {
				get_pool<EventType>( ).Unsubscribe( id );
			}

			/*! @brief Unsubscribes a previously registered permanent callback.
			*
			*  Removes the callback identified by SubscribtionID from the EventType pool.
			*
			*  @tparam EventType Type of event the callback was subscribed to.
			*  @param id Handle returned by SubscribePermanently().
			*  @thread_safety Call from the main thread or bus-owning thread.
			*/
			template< detail::tEvent EventType >
			void UnsubscribePermanent( detail::SubscribtionID id ) {
				get_pool<EventType>( ).UnsubscribePermanent( id );
			}

			/*! @brief Polls all event pools and executes pending events.
			*
			*  Iterates over all allocated EventPools and calls PollEvents on each to dispatch queued events.
			*
			*  @thread_safety Call from the main thread or the thread that owns the EventBus.
			*/
			void PollEvents( ) {
				for (usize i = 0; i < limits::gMaxEventTypes; i++) {
					if (mPools[ i ])
						mPools[ i ]->PollEvents( );
				}
			}



		private:

			template< detail::tEvent tType >
			detail::EventPool<tType>& get_pool( ) {
				auto typeID = GenerateTypeID::Get<tType>( );
				if (!mPools[ typeID ]) {
					mPools[ typeID ] = new detail::EventPool<tType>( );
				}
				return *static_cast< detail::EventPool<tType>* >(mPools[ typeID ]);
			}

			void init( ) {
				for (usize i = 0; i < limits::gMaxEventTypes; i++) {
					mPools[ i ] = nullptr;
				}
			}

			void destruct( ) {
				for (usize i = 0; i < limits::gMaxEventTypes; i++) {
					if (mPools[ i ] != nullptr) {
						delete mPools[ i ];
						mPools[ i ] = nullptr;
					}
				}
			}

			detail::BasePool* mPools[ limits::gMaxEventTypes ];

		};

	}
}