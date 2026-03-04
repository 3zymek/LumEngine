#pragma once
#include "core/core_common.hpp"
#include "core/core_pch.hpp"
#include "core/limits.hpp"
namespace lum {

	struct Event {};

	namespace ev::detail {

		template<typename tType>
		concept tEvent =
			std::is_base_of_v<Event, tType>&&
			std::is_trivially_copyable_v<tType>&&
			std::is_trivially_constructible_v<tType>;

		using EventT = uint32;
		using InvokeFn = void(*)(vptr userParam, cvptr event);
		using DestroyFn = void(*)(vptr userParam);
		using Storage = std::aligned_storage_t<256, alignof(std::max_align_t)>;
		using SubscribtionID = uint32;

		inline constexpr usize gMaxDeleteCallbacksPerFrame = 2;

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

			template<tEvent tType>
			constexpr static EventT Get( ) {
				static EventT typeID = gID++;
				return typeID;
			}

		private:

			static inline EventT gID = 0;

		};

	} // namespace lum::ev::detail

} // namespace lum
