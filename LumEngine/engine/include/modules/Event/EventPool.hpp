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
			mEventsCurrent.reserve( limits::kMaxCallbackPerFrame );
			mEventsNext.reserve( limits::kMaxCallbackPerFrame );
		}

		/* @brief Registers a one-shot callback. Destroyed automatically after first dispatch.
		*  @tparam tLambda Type of the callback lambda.
		*  @param lambda   Callback to invoke on event.
		*  @return SubscriptionID handle for manual unsubscription.
		*/
		template<typename tLambda>
		SubscriptionID Subscribe( tLambda&& lambda ) {
			setup_callback( std::forward<tLambda>( lambda ), mCallbacks[ mCallbackCount ] );
			return mCallbackCount++;
		}

		/* @brief Registers a persistent callback that survives across frames.
		*  @tparam tLambda Type of the callback lambda.
		*  @param lambda   Callback to invoke on event.
		*  @return SubscriptionID handle for manual unsubscription. Returns MaxValue on failure.
		*/
		template<typename tLambda>
		SubscriptionID SubscribePermanently( tLambda&& lambda ) {
			for (usize i = 0; i < limits::kMaxPermanentCallbacks; i++) {
				if (!mPermanentCallbacks[ i ].mActive) {
					setup_callback( std::forward<tLambda>( lambda ), mPermanentCallbacks[ i ] );
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
			if (mCallbackCount < id) return;
			auto& callback = mCallbacks[ id ];
			if (callback.mActive)
				callback.Destroy( );
		}

		/* @brief Destroys a permanent callback by ID.
		*  @param id SubscriptionID returned by SubscribePermanently().
		*/
		void UnsubscribePermanent( SubscriptionID id ) {
			auto& callback = mPermanentCallbacks[ id ];
			if (callback.mActive)
				callback.Destroy( );
		}

		/* @brief Queues an event for dispatch. Dropped silently if queue is full.
		*  @param event Event instance to queue.
		*/
		void Emit( const tType& event ) {
			if (mEventsCurrent.size( ) >= limits::kMaxEventEmitsPerFrame) return;
			if (!mFlushing)
				mEventsCurrent.push_back( event );
			else
				mEventsNext.push_back( event );
		}

		/* @brief Dispatches all queued events to subscribers.
		*  One-shot callbacks are destroyed after invocation.
		*  Events emitted during dispatch are deferred to the next flush.
		*/
		void FlushEvents( ) override {
			mFlushing = true;

			for (auto& event : mEventsCurrent)
				invoke_callbacks( event );

			mEventsCurrent.clear( );
			mFlushing = false;
			std::swap( mEventsCurrent, mEventsNext );
		}

	private:

		std::array<EventCallback, limits::kMaxCallbackPerFrame>   mCallbacks;
		std::array<EventCallback, limits::kMaxPermanentCallbacks> mPermanentCallbacks;
		SubscriptionID mCallbackCount = 0;

		std::vector<tType> mEventsCurrent;
		std::vector<tType> mEventsNext;

		bool mFlushing = false;

		/* @brief Stores a lambda in a callback slot using placement new.
		*  @tparam tLambda Type of the lambda to store.
		*  @param lambda   Lambda to move into the slot.
		*  @param callback Target callback slot to populate.
		*/
		template<typename tLambda>
		void setup_callback( tLambda&& lambda, EventCallback& callback ) {
			LUM_SASSERT( sizeof( LambdaStorage ) >= sizeof( tLambda ) && "Lambda too big for buffer" );
			LUM_SASSERT( alignof(LambdaStorage) >= alignof(tLambda) && "Lambda alignment mismatch" );

			new (&callback.mStorage) tLambda( std::forward<tLambda>( lambda ) );

			callback.mInvoke = []( vptr userParam, cvptr event ) {
				(*reinterpret_cast< tLambda* >(userParam))(*reinterpret_cast< const tType* >(event));
				};
			callback.mDestroy = []( vptr userParam ) {
				reinterpret_cast< tLambda* >(userParam)->~tLambda( );
				};
			callback.mActive = true;
		}

		/* @brief Invokes all active callbacks for the given event.
		*  One-shot callbacks are destroyed after invocation.
		*  @param event Event instance to dispatch.
		*/
		void invoke_callbacks( const tType& event ) {
			auto temp = mCallbackCount;

			for (int32 i = 0; i < temp; i++) {
				auto& callback = mCallbacks[ i ];
				if (!callback.mActive) continue;
				callback.mInvoke( &callback.mStorage, &event );
				callback.Destroy( );
			}
			mCallbackCount = 0;

			for (auto& callback : mPermanentCallbacks) {
				if (callback.mActive)
					callback.mInvoke( &callback.mStorage, &event );
			}
		}

	};

} // namespace lum::ev::detail