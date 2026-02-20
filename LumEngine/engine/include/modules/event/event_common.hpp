#pragma once
#include "core/core_common.hpp"
#include "core/core_pch.hpp"
#include "core/limits.hpp"
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

			LUM_COMPILE_VARIABLE usize gMaxDeleteCallbacksPerFrame = 2;

			struct Callback {
				Storage mStorage{};
				InvokeFn mInvoke = nullptr;
				DestroyFn mDestroy = nullptr;
				bool bActive = false;
				void Destroy() {
					(*mDestroy)(&mStorage);
					bActive = false;
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