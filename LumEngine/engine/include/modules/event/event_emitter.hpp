//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: RAII wrapper for EventBus managing permanent subscriptions.
//
//=============================================================================//
#pragma once
#include "event/event_common.hpp"
#include "event/event_bus.hpp"

namespace lum::ev {

	namespace detail {

		using UnsubscribeInvoke = void (*)(SubscribtionID, EventBus&);

		struct EmitterSlot {
			SubscribtionID    mID = MaxValue<SubscribtionID>( );
			UnsubscribeInvoke mUnsub = nullptr;
		};

	}

	// RAII wrapper for EventBus. Automatically unsubscribes permanent callbacks on destruction.
	class EventEmitter {

		using EmitterSlot = detail::EmitterSlot;
		using SubscribtionID = detail::SubscribtionID;

	public:

		explicit EventEmitter( EventBus& bus ) : mBus( bus ) {
			mSubscriptions.reserve( limits::gMaxPermanentCallbacks );
		}
		~EventEmitter( ) { destroy( ); }

		// Emits an event through the bound EventBus.
		template<detail::tEvent EventType>
		void Emit( const EventType& event ) {
			mBus.Emit<EventType>( event );
		}

		// Subscribes a one-shot callback. Removed automatically after first dispatch.
		template<detail::tEvent EventType, typename Lambda>
		void Subscribe( Lambda&& lambda ) {
			mBus.Subscribe<EventType>( std::forward<Lambda>( lambda ) );
		}

		// Subscribes a permanent callback. Unsubscribed automatically on destruction.
		// Ignored if gMaxPermanentCallbacks limit is reached.
		template<detail::tEvent EventType, typename Lambda>
		void SubscribePermamently( Lambda&& lambda ) {
			if (mSubscriptions.size( ) >= limits::gMaxPermanentCallbacks) return;

			SubscribtionID id = mBus.SubscribePermanently<EventType>( std::forward<Lambda>( lambda ) );
			mSubscriptions.push_back( { id, []( SubscribtionID id, EventBus& bus ) {
				bus.UnsubscribePermanent<EventType>( id );
			} } );
		}

	private:

		EventBus& mBus;
		std::vector<EmitterSlot> mSubscriptions;

		void destroy( ) {
			for (auto& slot : mSubscriptions)
				slot.mUnsub( slot.mID, mBus );
		}

	};

} // namespace lum::ev