#pragma once
#include "event/event_common.hpp"
#include "core/utils/lum_assert.hpp"
namespace lum {
	namespace ev {
		namespace detail {

			struct BasePool {
				virtual void PollEvents() = 0;
			};

			template<LumEvent T>
			class EventPool : public BasePool {

				using CallbackArray		= std::array<Callback, limits::gMaxCallbackPerFrame>;
				using PermCallbackArray = std::array<Callback, limits::gMaxPermanentCallbacks>;
				using SlotsVector		= std::vector<EventT>;
				using EventsVector		= std::vector<T>;

			public:

				EventPool() { Init(); }

				template<typename Lambda>
				SubscribtionID Subscribe(Lambda&& lambda) {

					SetupCallback(std::forward<Lambda>(lambda), mCallbacks[mCurrentCallbacksID]);

					LUM_LOG_DEBUG("Subscribed at slot {}");
					return mCurrentCallbacksID++;

				}

				template<typename Lambda>
				SubscribtionID SubscribePermanently(Lambda&& lambda) {

					if (mPermFreeSlots.empty()) {
						LUM_LOG_ERROR("Couldn't substribe - slots for permament callbacks are full");
						return std::numeric_limits<SubscribtionID>::max();
					}

					auto slot = mPermFreeSlots.back();
					mPermFreeSlots.pop_back();
					mPermActiveSlots.push_back(slot);

					SetupCallback(std::forward<Lambda>(lambda), mPermCallbacks[slot]);

					LUM_LOG_DEBUG("Subscribed permamently at slot {}");
					return slot;

				}

				void Unsubscribe(SubscribtionID id) {

					if (mCurrentCallbacksID < id) {
						LUM_LOG_WARN("Subscribtion at id {} does not exists");
						return;
					}

					auto& callback = mCallbacks[id];
					if (!callback.bActive)
						return;

					callback.Destroy();

					LUM_LOG_DEBUG("Unsubscribed callback at slot {}");

				}
				void UnsubscribePermanent(SubscribtionID id) {

					if (mPermActiveSlots.empty() || mToDelete.size() >= gMaxDeleteCallbacksPerFrame) {
						LUM_LOG_WARN("Unable to unsubscribe callback");
						return;
					}

					auto& callback = mPermCallbacks[id];
					callback.Destroy();
					mPermFreeSlots.push_back(id);

					for (size_t i = 0; i < mPermActiveSlots.size(); i++) {
						if (mPermActiveSlots[i] == id) {
							mToDelete.push_back(i);
							return;
						}
					}

				}

				void Emit(const T& event) {

					if (mEventsCurrent.size() >= limits::gMaxEventEmittsPerFrame)
						return;
					if (!bPolling)
						mEventsCurrent.push_back(event);
					else
						mEventsNext.push_back(event);
					LUM_LOG_DEBUG("Emitting event");

				}

				void PollEvents() override {

					bPolling = true;

					DeleteCallbacks();

					for (auto& event : mEventsCurrent) {
						InvokeCallbacks(event);
					}

					mEventsCurrent.clear();

					bPolling = false;

					std::swap(mEventsCurrent, mEventsNext);

				}

			private:

				template<typename Lambda>
				void SetupCallback(Lambda&& lambda, Callback& callback) {

					static_assert(sizeof(Storage) >= sizeof(Lambda), "Lambda too big for buffer");
					static_assert(alignof(Storage) >= alignof(Lambda), "Lambda aligment dismatch");
					new (&callback.mStorage) Lambda(std::forward<Lambda>(lambda));

					callback.mInvoke = [](vptr userParam, cvptr event) {
						auto* l = reinterpret_cast<Lambda*>(userParam);
						(*l)(*reinterpret_cast<const T*>(event));
						};
					callback.mDestroy = [](vptr userParam) {
						reinterpret_cast<Lambda*>(userParam)->~Lambda();
						};
					callback.bActive = true;

				}

				void DeleteCallbacks() {

					std::sort(mToDelete.begin(), mToDelete.end(), std::greater<>());

					for (auto& slot : mToDelete) {
						mPermCallbacks[mPermActiveSlots[slot]].Destroy();
						std::swap(mPermActiveSlots[slot], mPermActiveSlots.back());
						mPermActiveSlots.pop_back();
					}
					mToDelete.clear();

				}

				void InvokeCallbacks(const T& event) {

					auto temp = mCurrentCallbacksID;
					// Temporary callbacks
					for (EventT i = 0; i < temp; i++) {
						auto& callback = mCallbacks[i];
						if (!callback.bActive)
							continue;
						callback.mInvoke(&callback.mStorage, &event);
						callback.Destroy();
					}
					mCurrentCallbacksID = 0;

					// Permament Callbacks
					for (auto& slot : mPermActiveSlots) {
						auto& callback = mPermCallbacks[slot];
						callback.mInvoke(&callback.mStorage, &event);
					}

				}

				void Init() {

					mPermFreeSlots.reserve(limits::gMaxPermanentCallbacks);
					mPermActiveSlots.reserve(limits::gMaxPermanentCallbacks);
					mEventsCurrent.reserve(limits::gMaxCallbackPerFrame);
					mEventsNext.reserve(limits::gMaxCallbackPerFrame);
					mToDelete.reserve(gMaxDeleteCallbacksPerFrame);

					for (EventT i = 0; i < limits::gMaxPermanentCallbacks; i++) {
						mPermFreeSlots.push_back(i);
					}

				}

				CallbackArray	mCallbacks;
				EventT			mCurrentCallbacksID = 0;

				PermCallbackArray	mPermCallbacks;
				SlotsVector			mPermFreeSlots;
				SlotsVector			mPermActiveSlots;
				SlotsVector			mToDelete;

				EventsVector mEventsCurrent;
				EventsVector mEventsNext;

				bool bPolling = false;

			};
		}
	}
}