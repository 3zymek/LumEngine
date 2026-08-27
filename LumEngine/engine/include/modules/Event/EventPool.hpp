//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Typed event pool managing subscriptions and deferred dispatch.
//
//=============================================================================//
#pragma once
#include "Event/EventCommon.hpp"
#include "Core/Utils/LumAssert.hpp"
#include "Core/Limits.hpp"

namespace lum::ev::detail {

	/* @brief Abstract base for type-erased event pool storage. */
	struct EventPoolBase {
		virtual void FlushEvents( ) = 0;
	};

	/* @brief Manages subscriptions and deferred dispatch for a single event type.
	*
	* Stores one-shot and permanent callbacks in fixed-size arrays.
	* Events are queued and dispatched in bulk via FlushEvents().
	* Double-buffered to allow emitting events during dispatch.
	*
	* @tparam tType Event type this pool handles. Must satisfy cEvent concept.
	*/
	template<cEvent tType>
	class EventPool : public EventPoolBase {
	public:

		EventPool( ) {
			m_EventsCurrent.reserve( limits::k_MaxCallbackPerFrame );
			m_EventsNext.reserve( limits::k_MaxCallbackPerFrame );
		}

		/* @brief Registers a one-shot callback. Destroyed automatically after first dispatch.
		*  @tparam tLambda Type of the callback lambda.
		*  @param lambda   Callback to invoke on event.
		*  @return SubscriptionID handle for manual unsubscription.
		*/
		template<typename tLambda>
		SubscriptionID Subscribe( tLambda&& lambda ) {
			setup_callback( std::forward<tLambda>( lambda ), m_Callbacks[ m_CallbackCount ] );
			return m_CallbackCount++;
		}

		/* @brief Registers a persistent callback that survives across frames.
		*  @tparam tLambda Type of the callback lambda.
		*  @param lambda   Callback to invoke on event.
		*  @return SubscriptionID handle for manual unsubscription. Returns MaxValue on failure.
		*/
		template<typename tLambda>
		SubscriptionID SubscribePermanently( tLambda&& lambda ) {
			for (usize i = 0; i < limits::k_MaxPermanentCallbacks; i++) {
				if (!m_PermanentCallbacks[ i ].m_Active) {
					setup_callback( std::forward<tLambda>( lambda ), m_PermanentCallbacks[ i ] );
					return i;
				}
			}
			LUM_LOG_WARN( "No free permanent callback slots" );
			return MaxValue<SubscriptionID>( );
		}

		/* @brief Destroys a one-shot callback by ID.
		*  @param id SubscriptionID returned by Subscribe().
		*/
		void Unsubscribe( SubscriptionID id ) {
			if (m_CallbackCount < id) return;
			auto& callback = m_Callbacks[ id ];
			if (callback.m_Active)
				callback.Destroy( );
		}

		/* @brief Destroys a permanent callback by ID.
		*  @param id SubscriptionID returned by SubscribePermanently().
		*/
		void UnsubscribePermanent( SubscriptionID id ) {
			auto& callback = m_PermanentCallbacks[ id ];
			if (callback.m_Active)
				callback.Destroy( );
		}

		/* @brief Queues an event for dispatch. Dropped silently if queue is full.
		*  @param event Event instance to queue.
		*/
		void Emit( const tType& event ) {
			if (m_EventsCurrent.size( ) >= limits::k_MaxEventEmitsPerFrame) return;
			if (!m_Flushing)
				m_EventsCurrent.push_back( event );
			else
				m_EventsNext.push_back( event );
		}

		/* @brief Dispatches all queued events to subscribers.
		*  One-shot callbacks are destroyed after invocation.
		*  Events emitted during dispatch are deferred to the next flush.
		*/
		void FlushEvents( ) override {
			m_Flushing = true;

			for (auto& event : m_EventsCurrent)
				invoke_callbacks( event );

			m_EventsCurrent.clear( );
			m_Flushing = false;
			std::swap( m_EventsCurrent, m_EventsNext );
		}

	private:

		std::array<EventCallback, limits::k_MaxCallbackPerFrame>   m_Callbacks;
		std::array<EventCallback, limits::k_MaxPermanentCallbacks> m_PermanentCallbacks;
		SubscriptionID m_CallbackCount = 0;

		std::vector<tType> m_EventsCurrent;
		std::vector<tType> m_EventsNext;

		bool m_Flushing = false;

		/* @brief Stores a lambda in a callback slot using placement new.
		*  @tparam tLambda Type of the lambda to store.
		*  @param lambda   Lambda to move into the slot.
		*  @param callback Target callback slot to populate.
		*/
		template<typename tLambda>
		void setup_callback( tLambda&& lambda, EventCallback& callback ) {
			LUM_SASSERT( sizeof( LambdaStorage ) >= sizeof( tLambda ) && "Lambda too big for buffer" );
			LUM_SASSERT( alignof(LambdaStorage) >= alignof(tLambda) && "Lambda alignment mismatch" );

			new (&callback.m_Storage) tLambda( std::forward<tLambda>( lambda ) );

			callback.m_Invoke = []( void* userParam, const void* event ) {
				(*reinterpret_cast<tLambda*>(userParam))(*reinterpret_cast<const tType*>(event));
				};
			callback.m_Destroy = []( void* userParam ) {
				reinterpret_cast<tLambda*>(userParam)->~tLambda( );
				};
			callback.m_Active = true;
		}

		/* @brief Invokes all active callbacks for the given event.
		*  One-shot callbacks are destroyed after invocation.
		*  @param event Event instance to dispatch.
		*/
		void invoke_callbacks( const tType& event ) {
			auto temp = m_CallbackCount;

			for (int32 i = 0; i < temp; i++) {
				auto& callback = m_Callbacks[ i ];
				if (!callback.m_Active) continue;
				callback.m_Invoke( &callback.m_Storage, &event );
				callback.Destroy( );
			}
			m_CallbackCount = 0;

			for (auto& callback : m_PermanentCallbacks) {
				if (callback.m_Active)
					callback.m_Invoke( &callback.m_Storage, &event );
			}
		}

	};

} // namespace lum::ev::detail