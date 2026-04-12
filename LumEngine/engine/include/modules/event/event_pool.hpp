//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Typed event pool managing subscriptions and deferred dispatch.
//
//=============================================================================//
#pragma once
#include "event/event_common.hpp"
#include "core/utils/lum_assert.hpp"

namespace lum::ev::detail {

	// Abstract base for type-erased event pool storage.
	struct BasePool {
		virtual void PollEvents( ) = 0;
	};

	// Manages subscriptions and deferred dispatch for a single event type.
	template<tEvent tType>
	class EventPool : public BasePool {
	public:

		EventPool( ) {
			mEventsCurrent.reserve( limits::gMaxCallbackPf );
			mEventsNext.reserve( limits::gMaxCallbackPf );
		}

		// Registers a one-shot callback, destroyed after dispatch.
		template<typename tLambda>
		SubscribtionID Subscribe( tLambda&& lambda ) {
			setup_callback( std::forward<tLambda>( lambda ), mCallbacks[ mCurrentCallbacksID ] );
			return mCurrentCallbacksID++;
		}

		// Registers a persistent callback that survives across frames.
		template<typename tLambda>
		SubscribtionID SubscribePermanently( tLambda&& lambda ) {
			for (usize i = 0; i < limits::gMaxPermanentCallbacks; i++) {
				if (!mPermCallbacks[ i ].bActive) {
					setup_callback( std::forward<tLambda>( lambda ), mPermCallbacks[ i ] );
					return i;
				}
			}
			LUM_LOG_WARN( "No free permanent callback slots" );
			return MaxValue<SubscribtionID>( );
		}

		// Destroys a one-shot callback by ID.
		void Unsubscribe( SubscribtionID id ) {
			if (mCurrentCallbacksID < id) return;
			auto& callback = mCallbacks[ id ];
			if (callback.bActive)
				callback.Destroy( );
		}

		// Destroys a permanent callback by ID.
		void UnsubscribePermanent( SubscribtionID id ) {
			auto& callback = mPermCallbacks[ id ];
			if (callback.bActive)
				callback.Destroy( );
		}

		// Queues an event for dispatch. Dropped if queue is full.
		void Emit( const tType& event ) {
			if (mEventsCurrent.size( ) >= limits::gMaxEventEmittsPerFrame) return;
			if (!bPolling)
				mEventsCurrent.push_back( event );
			else
				mEventsNext.push_back( event );
		}

		// Dispatches all queued events to subscribers.
		void PollEvents( ) override {
			bPolling = true;

			for (auto& event : mEventsCurrent)
				invoke_callbacks( event );

			mEventsCurrent.clear( );
			bPolling = false;
			std::swap( mEventsCurrent, mEventsNext );
		}

	private:

		std::array<Callback, limits::gMaxCallbackPf>         mCallbacks;
		std::array<Callback, limits::gMaxPermanentCallbacks> mPermCallbacks;
		EventT mCurrentCallbacksID = 0;

		std::vector<tType> mEventsCurrent;
		std::vector<tType> mEventsNext;

		bool bPolling = false;

		// Stores lambda in callback slot using placement new.
		template<typename tLambda>
		void setup_callback( tLambda&& lambda, Callback& callback ) {
			LUM_SASSERT( sizeof( Storage ) >= sizeof( tLambda ) && "Lambda too big for buffer" );
			LUM_SASSERT( alignof(Storage) >= alignof(tLambda) && "Lambda alignment mismatch" );

			new (&callback.mStorage) tLambda( std::forward<tLambda>( lambda ) );

			callback.mInvoke = []( vptr userParam, cvptr event ) {
				(*reinterpret_cast< tLambda* >(userParam))(*reinterpret_cast< const tType* >(event));
				};
			callback.mDestroy = []( vptr userParam ) {
				reinterpret_cast< tLambda* >(userParam)->~tLambda( );
				};
			callback.bActive = true;
		}

		// Invokes all active callbacks for an event, then destroys one-shot callbacks.
		void invoke_callbacks( const tType& event ) {
			auto temp = mCurrentCallbacksID;

			for (int32 i = 0; i < temp; i++) {
				auto& callback = mCallbacks[ i ];
				if (!callback.bActive) continue;
				callback.mInvoke( &callback.mStorage, &event );
				callback.Destroy( );
			}
			mCurrentCallbacksID = 0;

			for (auto& callback : mPermCallbacks) {
				if (callback.bActive)
					callback.mInvoke( &callback.mStorage, &event );
			}
		}
	};

} // namespace lum::ev::detail