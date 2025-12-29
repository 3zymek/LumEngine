#pragma once
#include "event/ev_common.hpp"
namespace ev {
	namespace detail {

		class BasePool {};

		template<LumEvent T>
		class EventPool : public BasePool {

			using CallbackArr		= std::array<Callback<T>, MAX_CALLBACKS_PER_FRAME>;
			using PermCallbackArr	= std::array<Callback<T>, MAX_PERM_CALLBACKS>;
			using FreeSlots			= std::vector<Event_t>;

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
			void SubscribePermamently(Lambda&& lambda) {
				Event_t slot;
				if (!perm_free_slots.empty()) {
					slot = perm_free_slots.back();
					perm_free_slots.pop_back();
				}
				else {
					LOG_ERROR("Couldn't substribe - slots for permament callbacks are full");
					return;
				}
				
					
			}

			void Emit(const T& event) {
				for (size_t i = 0; i < current_callbacks_id; i++) {
					auto& callback = callbacks[i];
					(*callback.invoke)(&callback.buffer, &event);
					callback.Destroy();
				}
				
			}

		private:

			void Init() {
				perm_free_slots.reserve(MAX_PERM_CALLBACKS);
				for (Event_t i = 0; i < MAX_PERM_CALLBACKS; i++) {
					perm_free_slots.push_back(i);
				}
			}
			
			CallbackArr callbacks;
			Event_t		current_callbacks_id;

			PermCallbackArr perm_callbacks;
			FreeSlots		perm_free_slots;

		};
	}
}
