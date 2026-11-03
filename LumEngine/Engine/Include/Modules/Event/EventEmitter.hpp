//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: RAII wrapper for EventBus managing permanent subscriptions.
//
//=============================================================================//
#pragma once
#include "Event/EventCommon.hpp"
#include "Event/EventBus.hpp"

namespace lum::ev {

	namespace detail {

		/* @brief Function pointer type for unsubscribing a permanent callback from the EventBus. */
		using EventUnsubscribeFunction = void (*)(SubscriptionID, EventBus&);

		/* @brief Stores a subscription ID and its associated unsubscribe function pointer. */
		struct EventEmitterSlot {
			SubscriptionID				m_Id = MaxValue<SubscriptionID>( );
			EventUnsubscribeFunction	m_Unsub = nullptr;
		};

	} // namespace lum::ev::detail


	/* @brief RAII wrapper for EventBus that automatically unsubscribes permanent callbacks on destruction.
	*
	* Owns a set of permanent subscriptions and releases them when the listener goes out of scope.
	* One-shot subscriptions are forwarded directly to the EventBus without tracking.
	*
	* @note Requires a valid EventBus reference for the lifetime of this object.
	*/
	class LUM_API EventListener {

		using EventEmitterSlot = detail::EventEmitterSlot;
		using SubscriptionID = detail::SubscriptionID;

	public:

		/* @brief Constructs an EventListener bound to the given EventBus. */
		explicit EventListener( EventBus& bus ) : m_Bus( bus ) {
			m_Subscriptions.reserve( limits::k_MaxPermanentCallbacks );
		}

		~EventListener( ) { unsubscribe_all( ); }

		/* @brief Emits an event through the bound EventBus.
		*  @tparam tEvent Event type to emit. Must satisfy cEvent concept.
		*  @param event Reference to the event instance to dispatch.
		*/
		template<detail::cEvent tEvent>
		void Emit( const tEvent& event ) {
			m_Bus.Emit<tEvent>( event );
		}

		/* @brief Subscribes a one-shot callback. Removed automatically after first dispatch.
		*  @tparam tEvent  Event type to subscribe to.
		*  @tparam Lambda  Type of the callback lambda.
		*  @param lambda   Callback to invoke on event.
		*/
		template<detail::cEvent tEvent, typename Lambda>
		void Subscribe( Lambda&& lambda ) {
			m_Bus.Subscribe<tEvent>( std::forward<Lambda>( lambda ) );
		}

		/* @brief Subscribes a permanent callback. Unsubscribed automatically on destruction.
		*  Ignored if kMaxPermanentCallbacks limit is reached.
		*  @tparam tEvent  Event type to subscribe to.
		*  @tparam Lambda  Type of the callback lambda.
		*  @param lambda   Callback to invoke on event.
		*/
		template<detail::cEvent tEvent, typename Lambda>
		void SubscribePermanently( Lambda&& lambda ) {
			if (m_Subscriptions.size( ) >= limits::k_MaxPermanentCallbacks) return;

			SubscriptionID id = m_Bus.SubscribePermanently<tEvent>( std::forward<Lambda>( lambda ) );
			m_Subscriptions.push_back( { id, []( SubscriptionID id, EventBus& bus ) {
				bus.UnsubscribePermanent<tEvent>( id );
			} } );
		}

	private:

		EventBus& m_Bus;
		std::vector<EventEmitterSlot> m_Subscriptions;

		/* @brief Unsubscribes all tracked permanent callbacks from the EventBus. */
		void unsubscribe_all( ) {
			for (auto& slot : m_Subscriptions)
				slot.m_Unsub( slot.m_Id, m_Bus );
		}

	};

} // namespace lum::ev