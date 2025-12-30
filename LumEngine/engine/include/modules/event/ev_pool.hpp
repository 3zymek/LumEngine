#pragma once
#include "event/ev_common.hpp"
namespace ev {
	namespace detail {

		class BasePool {};

		template<LumEvent T>
		class EventPool : public BasePool {

			using CallbackArr		= std::array<Callback<T>, MAX_CALLBACKS_PER_FRAME>;
			using PermCallbackArr	= std::array<Callback<T>, MAX_PERM_CALLBACKS>;
			using Slots			= std::vector<Event_t>;

		public:

			EventPool() { Init(); }

			template<typename Lambda>
			void Subscribe(Lambda&& lambda) {

				auto& slot = callbacks[current_callbacks_id++];

				new (slot.buffer) Lambda(std::forward<Lambda>(lambda));
				slot.invoke = [](void* userParam, const void* event) {
					auto* l = reinterpret_cast<Lambda*>(userParam);
					(*l)(*reinterpret_cast<const T*>(event));
				};
				slot.destroy = [](void* userParam) {
					reinterpret_cast<Lambda*>(userParam)->~Lambda();
				};

			}

			template<typename Lambda>
			[[nodiscard]] SubscribtionHandle SubscribePermamently(Lambda&& lambda) {
				if (perm_free_slots.empty()) {
					LOG_ERROR("Couldn't substribe - slots for permament callbacks are full");
					return std::numeric_limits<SubscribtionHandle>::max();
				}
				SubscribtionHandle slot = perm_free_slots.back();
				perm_free_slots.pop_back();
				perm_active_slots.push_back(slot);
				
				auto& callback = perm_callbacks[slot];

				new (&callback.buffer) Lambda(std::forward<Lambda>(lambda));
				callback.invoke = [](void* userParam, const void* event) {
					auto* l = reinterpret_cast<Lambda*>(userParam);
					(*l)(*reinterpret_cast<const T*>(event));
				};
				callback.destroy = [](void* userParam) {
					reinterpret_cast<Lambda*>(userParam)->~Lambda();
				};
				LOG_DEBUG(std::format("Subscribed permamently at slot {}", slot));
				return slot;
			}

			void Emit(const T& event) {
				for (Event_t i = 0; i < current_callbacks_id; i++) {
					auto& callback = callbacks[i];
					(*callback.invoke)(&callback.buffer, &event);
					callback.Destroy();
				}
				current_callbacks_id = 0;
				for (auto& slot : perm_active_slots) {
					auto& callback = perm_callbacks[slot];
					(*callback.invoke)(&callback.buffer, &event);
					callback.Destroy();
				}

			}

		private:

			void Init() {
				perm_free_slots.reserve(MAX_PERM_CALLBACKS);
				perm_active_slots.reserve(MAX_PERM_CALLBACKS);
				for (Event_t i = 0; i < MAX_PERM_CALLBACKS; i++) {
					perm_free_slots.push_back(i);
				}
			}
			
			CallbackArr callbacks;
			Event_t		current_callbacks_id = 0;

			PermCallbackArr perm_callbacks;
			Slots			perm_free_slots;
			Slots			perm_active_slots;

		};
	}
}
