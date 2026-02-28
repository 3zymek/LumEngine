//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Entity and component primitives for the ECS.
//
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"

namespace lum {

	/* @brief Base tag struct for all ECS components.
	* Inherit from this to mark a struct as a valid component.
	*/
	struct Component : std::true_type {};

	/* @brief Entity Component System namespace.
	*  Provides core ECS primitives including entity management,
	*  component storage and system iteration.
	*/
	namespace ecs {

		/* @brief Unique identifier for an entity. */
		using EntityID = uint64;


		/* @brief Internal implementation details for the ECS.
		*  Not intended for direct use outside of the ECS subsystem.
		*/
		namespace detail {

			/* @brief Concept validating a well-formed ECS component type.
			* Requires the type to inherit from lum::Component,
			* be trivially copyable and trivially destructible.
			*/
			template<typename tType>
			concept Component =
				std::is_base_of_v<lum::Component, tType>&&
				std::is_trivially_copyable_v<tType>&&
				std::is_trivially_destructible_v<tType>;

		} // namespace lum::ecs::detail

	} // namespace lum::ecs

} // namespace lum