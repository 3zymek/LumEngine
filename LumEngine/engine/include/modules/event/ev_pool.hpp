#pragma once
#include "core/core_pch.hpp"
#include "event/ev_common.hpp"
namespace ev {
	namespace detail {

		struct BasePool {};

		template<LumEvent EventType>
		class EventPool : public BasePool {

			using EventArr		= std::array<EventType, MAX_EVENTS_PER_FRAME>;
			using CallbacksArr	= std::array<Callback<EventType>, MAX_CALLBACKS_PER_FRAME>;
			using PCallbacksArr = std::array<Callback<EventType>, MAX_PERM_CALLBACKS>;

		public:

			template<typename Lambda>
			void Subscribe(Lambda&& lambda) {
				if (callbacks_index >= m_callbacks.max_size())
					return;

				auto& callback = m_callbacks[callbacks_index++];
				new (&callback.buffer) Lambda(std::forward<Lambda>(lambda));
				callback.invoke = [](void* ptr, const void* ev) {
					auto* l = reinterpret_cast<Lambda*>(ptr);
					(*l)(*reinterpret_cast<const EventType*>(ev));
				};
				callback.destroy = [](void* ptr) {
					(reinterpret_cast<Lambda*>(ptr))->~Lambda;
				};
				callback.active = true;

			}

			void Emit(const EventType& ev) {
				if (ev_index >= m_events.max_size())
					return;

				m_events[ev_index++] = std::move(ev);

			}


		private:

			Event_t			ev_index = 0;
			EventArr		m_events;

			Event_t			callbacks_index = 0;
			CallbacksArr	m_callbacks;

			Event_t			perm_callbacks_index = 0;
			PCallbacksArr	m_perm_callbacks;

		};
	}
}
