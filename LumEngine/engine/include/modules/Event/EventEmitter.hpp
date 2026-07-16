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
			SubscriptionID				mId = MaxValue<SubscriptionID>( );
			EventUnsubscribeFunction	mUnsub = nullptr;
		};

	} // namespace lum::ev::detail


	/* @brief RAII wrapper for EventBus that automatically unsubscribes permanent callbacks on destruction.
	*
	* Owns a set of permanent subscriptions and releases them when the listener goes out of scope.
	* One-shot subscriptions are forwarded directly to the EventBus without tracking.
	*
	* @note Requires a valid EventBus reference for the lifetime of this object.
	*/
	class EventListener {

		using EventEmitterSlot = detail::EventEmitterSlot;
		using SubscriptionID = detail::SubscriptionID;

	public:

		/* @brief Constructs an EventListener bound to the given EventBus. */
		explicit EventListener( EventBus& bus ) : mBus( bus ) {
			mSubscriptions.reserve( limits::kMaxPermanentCallbacks );
		}

		~EventListener( ) { unsubscribe_all( ); }

		/* @brief Emits an event through the bound EventBus.
		*  @tparam tEvent Event type to emit. Must satisfy cEvent concept.
		*  @param event Reference to the event instance to dispatch.
		*/
		template<detail::cEvent tEvent>
		void Emit( const tEvent& event ) {
			mBus.Emit<tEvent>( event );
		}

		/* @brief Subscribes a one-shot callback. Removed automatically after first dispatch.
		*  @tparam tEvent  Event type to subscribe to.
		*  @tparam Lambda  Type of the callback lambda.
		*  @param lambda   Callback to invoke on event.
		*/
		template<detail::cEvent tEvent, typename Lambda>
		void Subscribe( Lambda&& lambda ) {
			mBus.Subscribe<tEvent>( std::forward<Lambda>( lambda ) );
		}

		/* @brief Subscribes a permanent callback. Unsubscribed automatically on destruction.
		*  Ignored if kMaxPermanentCallbacks limit is reached.
		*  @tparam tEvent  Event type to subscribe to.
		*  @tparam Lambda  Type of the callback lambda.
		*  @param lambda   Callback to invoke on event.
		*/
		template<detail::cEvent tEvent, typename Lambda>
		void SubscribePermanently( Lambda&& lambda ) {
			if (mSubscriptions.size( ) >= limits::kMaxPermanentCallbacks) return;

			SubscriptionID id = mBus.SubscribePermanently<tEvent>( std::forward<Lambda>( lambda ) );
			mSubscriptions.push_back( { id, []( SubscriptionID id, EventBus& bus ) {
				bus.UnsubscribePermanent<tEvent>( id );
			} } );
		}

	private:

		EventBus& mBus;
		std::vector<EventEmitterSlot> mSubscriptions;

		/* @brief Unsubscribes all tracked permanent callbacks from the EventBus. */
		void unsubscribe_all( ) {
			for (auto& slot : mSubscriptions)
				slot.mUnsub( slot.mId, mBus );
		}

	};

} // namespace lum::ev