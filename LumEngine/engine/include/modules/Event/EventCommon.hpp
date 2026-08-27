//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core event system primitives — types, concepts and callback storage.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"
#include "Core/Limits.hpp"
#include "Core/Types.hpp"

namespace lum {

	/* @brief Base tag struct for all engine events.
	*  Inherit from this to mark a struct as a valid event type.
	*/
	struct LUM_API Event {};

	/* @brief Internal implementation details for the event system.
	*  Not intended for direct use outside of the event subsystem.
	*/
	namespace ev::detail {

		/* @brief Concept validating a well-formed engine event type.
		*  Requires the type to inherit from lum::Event,
		*  be trivially copyable and trivially constructible.
		*/
		template<typename tType>
		concept cEvent =
			std::is_base_of_v<Event, tType> &&
			std::is_copy_constructible_v<tType>;

		/* @brief Underlying type for event type identifiers. */
		using EventTypeID = uint32;

		/* @brief Function pointer type for invoking a stored callback. */
		using InvokeFunction = void(*)(void* userParam, const void* event);

		/* @brief Function pointer type for destroying a stored callback. */
		using DestroyFunction = void(*)(void* userParam);

		/* @brief Aligned storage buffer for type-erased lambda storage.
		*  Sized to 256 bytes with maximum natural alignment.
		*/
		using LambdaStorage = std::aligned_storage_t<256, alignof(std::max_align_t)>;

		/* @brief Handle type returned by Subscribe and SubscribePermanently. */
		using SubscriptionID = uint32;

		/* @brief Type-erased callback slot storing a lambda, invoke and destroy pointers. */
		struct EventCallback {

			LambdaStorage	m_Storage{};          /* @brief Raw storage for the captured lambda. */
			InvokeFunction  m_Invoke = nullptr;  /* @brief Pointer to the lambda invocation wrapper. */
			DestroyFunction m_Destroy = nullptr;  /* @brief Pointer to the lambda destructor wrapper. */
			bool			m_Active = false;    /* @brief Whether this slot is currently occupied. */

			/* @brief Destroys the stored lambda and marks the slot as inactive. */
			void Destroy( ) {
				(*m_Destroy)(&m_Storage);
				m_Active = false;
			}
		};

	} // namespace lum::ev::detail

} // namespace lum