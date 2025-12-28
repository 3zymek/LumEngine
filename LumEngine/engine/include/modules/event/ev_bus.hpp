#pragma once
#include "core/core_pch.hpp"
#include "event/ev_common.hpp"
#include "event/ev_pool.hpp"
namespace ev {

	static inline constexpr unsigned int MAX_EVENT_TYPES = 100;

	template<typename T>
	concept LumEvent = requires {
		T::__lumevent__ == true;
	};

	struct Callback {
		alignas(alignof(std::max_align_t)) char buffer[256];
		using InvokeFn = void(*)(void*, const void*);
		using DestroyFn = void(*)(void*);
		InvokeFn invoke = nullptr;
		DestroyFn destroy = nullptr;
		bool active = false;
		void Destroy() {
			destroy(&buffer);
			invoke = nullptr;
			destroy = nullptr;
		}
	};

	class EventBus {
	public:
		template<LumEvent EventType, typename Lambda>
		void Subscribe(Lambda&& lambda) {

			



		}
	private:

		template<LumEvent EventType>
		std::array<EventType, MAX_EVENT_TYPES>& GetPool() {
			static std::array<EventType, MAX_EVENT_TYPES> pool;
			return pool;
		}

	};



}
#include "ev_bus.ipp"
