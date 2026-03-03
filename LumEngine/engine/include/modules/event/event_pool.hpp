#pragma once
#include "event/event_common.hpp"
#include "core/utils/lum_assert.hpp"
namespace lum {
	namespace ev {
		namespace detail {

			struct BasePool {
				virtual void PollEvents() = 0;
			};

			template<LumEvent tType>
			class EventPool : public BasePool {

			public:

				EventPool() { init(); }

				template<typename tLambda>
				SubscribtionID Subscribe(tLambda&& lambda) {

					

					setup_callback(std::forward<tLambda>(lambda), mCallbacks[mCurrentCallbacksID]);

					return mCurrentCallbacksID++;

				}

				template<typename tLambda>
				SubscribtionID SubscribePermanently(tLambda&& lambda) {

					for (usize i = 0; i < limits::gMaxPermanentCallbacks; i++) {
						if (!mPermCallbacks[i].bActive) {
							setup_callback(std::forward<tLambda>(lambda), mPermCallbacks[i]);
							return i;
						}
					}
					LUM_LOG_WARN("No free permanent callback slots");
					return MaxVal<SubscribtionID>();

				}

				void Unsubscribe( SubscribtionID id ) {

					if (mCurrentCallbacksID < id) {
						return;
					}

					auto& callback = mCallbacks[id];
					if (callback.bActive)
						callback.Destroy();

				}
				void UnsubscribePermanent( SubscribtionID id ) {

					auto& callback = mPermCallbacks[id];
					if(callback.bActive)
						callback.Destroy();

				}

				void Emit(const tType& event) {

					if (mEventsCurrent.size() >= limits::gMaxEventEmittsPerFrame)
						return;
					if (!bPolling)
						mEventsCurrent.push_back(event);
					else
						mEventsNext.push_back(event);

				}

				void PollEvents() override {

					bPolling = true;

					for (auto& event : mEventsCurrent) {
						invoke_callbacks(event);
					}

					mEventsCurrent.clear();

					bPolling = false;

					std::swap(mEventsCurrent, mEventsNext);

				}

			private:

				template<typename tLambda>
				void setup_callback(tLambda&& lambda, Callback& callback) {

					LUM_SASSERT(sizeof(Storage) >= sizeof(tLambda) && "Lambda too big for buffer");
					LUM_SASSERT(alignof(Storage) >= alignof(tLambda) && "Lambda aligment dismatch");
					new (&callback.mStorage) tLambda(std::forward<tLambda>(lambda));

					callback.mInvoke = [](vptr userParam, cvptr event) {
						auto* l = reinterpret_cast<tLambda*>(userParam);
						(*l)(*reinterpret_cast<const tType*>(event));
						};
					callback.mDestroy = [](vptr userParam) {
						reinterpret_cast<tLambda*>(userParam)->~Lambda();
						};
					callback.bActive = true;

				}

				void invoke_callbacks(const tType& event) {

					auto temp = mCurrentCallbacksID;
					// Temporary callbacks
					for (int32 i = 0; i < temp; i++) {
						auto& callback = mCallbacks[i];
						if (!callback.bActive)
							continue;
						callback.mInvoke(&callback.mStorage, &event);
						callback.Destroy();
					}
					mCurrentCallbacksID = 0;

					// Permament Callbacks
					for (auto& slot : mPermCallbacks) {
						auto& callback = mPermCallbacks[slot];
						if(callback.bActive)
							callback.mInvoke(&callback.mStorage, &event);
					}

				}

				void init() {

					mEventsCurrent.reserve(limits::gMaxCallbackPf);
					mEventsNext.reserve(limits::gMaxCallbackPf);

				}

				std::array<Callback, limits::gMaxCallbackPf>    mCallbacks;
				std::array<Callback, limits::gMaxPermanentCallbacks>  mPermCallbacks;
				EventT mCurrentCallbacksID = 0;

				std::vector<tType> mEventsCurrent;
				std::vector<tType> mEventsNext;

				bool bPolling = false;

			};
		}
	}
}