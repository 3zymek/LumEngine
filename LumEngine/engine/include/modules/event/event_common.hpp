#pragma once
#include "core/core_common.hpp"
#include "core/core_pch.hpp"
namespace lum {
	namespace ev {
		namespace detail {

			template<typename T>
			concept LumEvent =
				requires { T::__lumevent__ == true; }&&
			std::is_trivially_copyable_v<T>&&
				std::is_trivially_constructible_v<T>;

			using Event_t	= uint32;
			using InvokeFn	= void(*)(vptr userParam, cvptr event);
			using DestroyFn = void(*)(vptr userParam);
			using Storage	= std::aligned_storage_t<256, alignof(std::max_align_t)>;
			
			using SubscribtionID = uint32;

			inline constexpr usize MAX_EVENT_TYPES = settings::EVENT_MAX_EVENT_TYPES;
			inline constexpr usize MAX_CALLBACKS_PER_FRAME = settings::EVENT_MAX_CALLBACKS_PER_FRAME;
			inline constexpr usize MAX_PERM_CALLBACKS = settings::EVENT_MAX_PERMAMENT_CALLBACKS;
			inline constexpr usize MAX_EMITTS_PER_FRAME = settings::EVENT_MAX_EMITTS_PER_FRAME;
			inline constexpr usize MAX_TO_DELETE_CALLBACKS_PER_FRAME = 2;

			struct Callback {
				Storage buffer{};
				InvokeFn invoke = nullptr;
				DestroyFn destroy = nullptr;
				bool active = false;
				void Destroy() {
					(*destroy)(&buffer);
					active = false;
				}
			};

			struct GetEventTypeID {
				template<LumEvent T>
				constexpr static Event_t Get() {
					static Event_t typeID = globalID++;
					return typeID;
				}
			private:

				static inline Event_t globalID = 0;

			};


			#define LumEventTag static constexpr bool __lumevent__ = true;

		}
	}
}