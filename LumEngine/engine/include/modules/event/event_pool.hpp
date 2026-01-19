#pragma once
#include "event/event_common.hpp"
#include "core/lum_assert.hpp"
namespace lum {
	namespace ev {
		namespace detail {

			struct BasePool {
				virtual void PollEvents() = 0;
			};

			template<LumEvent T>
			class EventPool : public BasePool {

				using CallbackArray		= std::array<Callback, MAX_CALLBACKS_PER_FRAME>;
				using PermCallbackArray = std::array<Callback, MAX_PERM_CALLBACKS>;
				using SlotsVector		= std::vector<Event_t>;
				using EventsVector		= std::vector<T>;

			public:

				EventPool() { Init(); }

				template<typename Lambda>
				SubscribtionID Subscribe(Lambda&& lambda) {

					SetupCallback(std::forward<Lambda>(lambda), callbacks[current_callbacks_id]);

					LUM_LOG_DEBUG(std::format("Subscribed at slot {}", current_callbacks_id));
					return current_callbacks_id++;

				}

				template<typename Lambda>
				SubscribtionID SubscribePermanently(Lambda&& lambda) {

					if (perm_free_slots.empty()) {
						LUM_LOG_ERROR("Couldn't substribe - slots for permament callbacks are full");
						return std::numeric_limits<SubscribtionID>::max();
					}

					auto slot = perm_free_slots.back();
					perm_free_slots.pop_back();
					perm_active_slots.push_back(slot);

					SetupCallback(std::forward<Lambda>(lambda), perm_callbacks[slot]);

					LUM_LOG_DEBUG(std::format("Subscribed permamently at slot {}", slot));
					return slot;

				}

				void Unsubscribe(SubscribtionID id) {

					if (current_callbacks_id < id) {
						LUM_LOG_WARN(std::format("Subscribtion at id {} does not exists", id));
						return;
					}

					auto& callback = callbacks[id];
					if (!callback.active)
						return;

					callback.Destroy();

					LUM_LOG_DEBUG(std::format("Unsubscribed callback at slot {}", id));

				}
				void UnsubscribePermanent(SubscribtionID id) {

					if (perm_active_slots.empty() || to_delete.size() >= MAX_TO_DELETE_CALLBACKS_PER_FRAME) {
						LUM_LOG_WARN("Unable to unsubscribe callback");
						return;
					}

					auto& callback = perm_callbacks[id];
					callback.Destroy();
					perm_free_slots.push_back(id);

					for (size_t i = 0; i < perm_active_slots.size(); i++) {
						if (perm_active_slots[i] == id) {
							to_delete.push_back(i);
							return;
						}
					}

				}

				void Emit(const T& event) {

					if (m_events_current.size() >= MAX_EMITTS_PER_FRAME)
						return;
					if (!polling)
						m_events_current.push_back(event);
					else
						m_events_next.push_back(event);
					LUM_LOG_DEBUG("Emitting event");

				}

				void PollEvents() override {

					polling = true;

					DeleteCallbacks();

					for (auto& event : m_events_current) {
						InvokeCallbacks(event);
					}

					m_events_current.clear();

					polling = false;

					std::swap(m_events_current, m_events_next);

				}

			private:

				template<typename Lambda>
				void SetupCallback(Lambda&& lambda, Callback& callback) {

					static_assert(sizeof(Storage) >= sizeof(Lambda), "Lambda too big for buffer");
					static_assert(alignof(Storage) >= alignof(Lambda), "Lambda aligment dismatch");
					new (&callback.buffer) Lambda(std::forward<Lambda>(lambda));

					callback.invoke = [](LUMvptr userParam, LUMcvptr event) {
						auto* l = reinterpret_cast<Lambda*>(userParam);
						(*l)(*reinterpret_cast<const T*>(event));
						};
					callback.destroy = [](LUMvptr userParam) {
						reinterpret_cast<Lambda*>(userParam)->~Lambda();
						};
					callback.active = true;

				}

				void DeleteCallbacks() {

					std::sort(to_delete.begin(), to_delete.end(), std::greater<>());

					for (auto& slot : to_delete) {
						perm_callbacks[perm_active_slots[slot]].Destroy();
						std::swap(perm_active_slots[slot], perm_active_slots.back());
						perm_active_slots.pop_back();
					}
					to_delete.clear();

				}

				void InvokeCallbacks(const T& event) {

					auto temp = current_callbacks_id;
					// Temporary callbacks
					for (Event_t i = 0; i < temp; i++) {
						auto& callback = callbacks[i];
						if (!callback.active)
							continue;
						callback.invoke(&callback.buffer, &event);
						callback.Destroy();
					}
					current_callbacks_id = 0;

					// Permament Callbacks
					for (auto& slot : perm_active_slots) {
						auto& callback = perm_callbacks[slot];
						callback.invoke(&callback.buffer, &event);
					}

				}

				void Init() {

					perm_free_slots.reserve(MAX_PERM_CALLBACKS);
					perm_active_slots.reserve(MAX_PERM_CALLBACKS);
					m_events_current.reserve(MAX_EMITTS_PER_FRAME);
					m_events_next.reserve(MAX_EMITTS_PER_FRAME);
					to_delete.reserve(MAX_TO_DELETE_CALLBACKS_PER_FRAME);

					for (Event_t i = 0; i < MAX_PERM_CALLBACKS; i++) {
						perm_free_slots.push_back(i);
					}

				}

				CallbackArray	callbacks;
				Event_t			current_callbacks_id = 0;

				PermCallbackArray	perm_callbacks;
				SlotsVector			perm_free_slots;
				SlotsVector			perm_active_slots;
				SlotsVector			to_delete;

				EventsVector m_events_current;
				EventsVector m_events_next;

				bool polling = false;

			};
		}
	}
}