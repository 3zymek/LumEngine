#pragma once
#include <iostream>
#include <functional>
#include "utils/e_define.hpp"
#include "eventmodule/details/ev_define.hpp"
#include "eventmodule/details/ev_pool.hpp"
namespace ev {

	class EventBus {
	public:

		EventBus()	{ Init(); }
		~EventBus() { Kill(); }

		template<detail::EventT T>
		void RegisterPool();

		template<detail::EventT T, typename Arg>
		detail::CallbackID Subscribe(Arg&& args);

		template<detail::EventT T, typename Arg>
		detail::CallbackID SubscribePermamently(Arg&& args);

		template<detail::EventT T>
		void Unsubscribe(const detail::CallbackID&);

		template<detail::EventT T>
		void Emit(const T& event);

		void ProcessAll();

		static EventBus& Engine() {
			static EventBus eb;
			return eb;
		}

	private:

		template<detail::EventT T>
		detail::EventPool<T>& GetPool();

		void Kill();

		void Init();

		detail::BasePool* m_pools[detail::MAX_EVENT_TYPES];

	};

}
#include "ev_bus.ipp"
