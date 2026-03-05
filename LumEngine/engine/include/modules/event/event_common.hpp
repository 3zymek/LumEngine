//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Core event system primitives — types, concepts and callback storage.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"
#include "core/limits.hpp"

namespace lum {

	/* @brief Base tag struct for all engine events.
	*  Inherit from this to mark a struct as a valid event type.
	*/
	struct Event {};

	/* @brief Internal implementation details for the event system.
	*  Not intended for direct use outside of the event subsystem.
	*/
	namespace ev::detail {

		/* @brief Concept validating a well-formed engine event type.
		*  Requires the type to inherit from lum::Event,
		*  be trivially copyable and trivially constructible.
		*/
		template<typename tType>
		concept tEvent =
			std::is_base_of_v<Event, tType>&&
			std::is_trivially_copyable_v<tType>&&
			std::is_trivially_constructible_v<tType>;

		/* @brief Underlying type for event type identifiers. */
		using EventT = uint32;

		/* @brief Function pointer type for invoking a stored callback. */
		using InvokeFn = void(*)(vptr userParam, cvptr event);

		/* @brief Function pointer type for destroying a stored callback. */
		using DestroyFn = void(*)(vptr userParam);

		/* @brief Aligned storage buffer for type-erased lambda storage.
		*  Sized to 256 bytes with maximum natural alignment.
		*/
		using Storage = std::aligned_storage_t<256, alignof(std::max_align_t)>;

		/* @brief Handle type returned by Subscribe and SubscribePermanently. */
		using SubscribtionID = uint32;

		/* @brief Type-erased callback slot storing a lambda, invoke and destroy pointers. */
		struct Callback {

			Storage   mStorage{};          /* @brief Raw storage for the captured lambda. */
			InvokeFn  mInvoke = nullptr;  /* @brief Pointer to the lambda invocation wrapper. */
			DestroyFn mDestroy = nullptr;  /* @brief Pointer to the lambda destructor wrapper. */
			bool      bActive = false;    /* @brief Whether this slot is currently occupied. */

			/* @brief Destroys the stored lambda and marks the slot as inactive. */
			void Destroy( ) {
				(*mDestroy)(&mStorage);
				bActive = false;
			}
		};

	} // namespace lum::ev::detail

} // namespace lum