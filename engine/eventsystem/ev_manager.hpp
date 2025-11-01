#pragma once
#include <iostream>
#include <functional>
#include "ev_base.hpp"
#include "e_define.hpp"
#include "ev_define.hpp"
#include "imanager.hpp"
#include "ev_pool.hpp"
namespace ev {

	class EventManager {
	public:

		EventManager()	{ Init(); }
		~EventManager() { Kill(); }

		template<detail::EventT T>
		void RegisterPool();

		template<detail::EventT T, typename Arg>
		NODISCARD detail::CallbackID Subscribe(Arg&& args);

		template<detail::EventT T, typename Arg>
		NODISCARD detail::CallbackID SubscribePermamently(Arg&& args);

		template<detail::EventT T>
		void Unsubscribe(const detail::CallbackID&);

		template<detail::EventT T>
		void Emit(const T& event);

		void ProcessAll();

	private:

		template<detail::EventT T>
		detail::EventPool<T>& GetPool();

		void Kill();

		void Init();

		detail::BasePool* m_pools[detail::MAX_EVENT_TYPES];

	};

}
#include "ev_manager.ipp"
