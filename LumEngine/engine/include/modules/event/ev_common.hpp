#pragma once
#include "core/core_common.hpp"
#include "core/core_pch.hpp"
namespace ev {
	namespace detail {

		template<typename T>
		concept LumEvent = requires { T::__lumevent__ == true; };

		using Event_t	= unsigned int;
		using InvokeFn	= void(*)(void* userParam, const void* event);
		using DestroyFn = void(*)(void* userParam);

		inline constexpr Event_t MAX_EVENT_TYPES = settings::EVENT_MAX_EVENT_TYPES;
		inline constexpr Event_t MAX_CALLBACKS_PER_FRAME = settings::MAX_CALLBACKS_PER_FRAME;
		inline constexpr Event_t MAX_PERM_CALLBACKS = settings::MAX_PERMAMENT_CALLBACKS;

		template<LumEvent T>
		struct Callback {
			alignas(alignof(std::max_align_t)) char buffer[256]{};
			InvokeFn invoke = nullptr;
			DestroyFn destroy = nullptr;
			void Destroy() {
				(*destroy)(&buffer);
			}
		};

		struct GetEventTypeID {
			template<LumEvent T>
			static Event_t Get() {
				static Event_t typeID = globalID++;
				return typeID;
			}
		private:

			static inline Event_t globalID = 0;

		};


		#define LumEventTag static constexpr inline bool __lumevent__ = true;

	}
}