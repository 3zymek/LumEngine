//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Entity and component primitives for the ECS.
//
//=============================================================================//
#pragma once
#include "Core/CoreCommon.hpp"

namespace lum {

	/* @brief Base tag struct for all ECS components.
	* Inherit from this to mark a struct as a valid component.
	*/
	struct ComponentBase {
		/* @brief Defines if component was changed. */
		bool mMarked = true;
	};

	/* @brief Unique identifier for an entity. */
	using EntityID = uint64;

	/* @brief Unique numeric identifier for a component type, assigned at runtime. */
	using ComponentTypeID = usize;

	/* @brief Entity Component System namespace.
	*  Provides core ECS primitives including entity management,
	*  component storage and system iteration.
	*/
	namespace ecs {

		/* @brief Internal implementation details for the ECS.
		*  Not intended for direct use outside of the ECS subsystem.
		*/
		namespace detail {

			/* @brief Concept validating a well-formed ECS component type.
			* Requires the type to inherit from lum::Component,
			* be trivially copyable and trivially destructible.
			*/
			template<typename tType>
			concept cComponent = std::is_base_of_v<lum::ComponentBase, tType>;

		} // namespace lum::ecs::detail

		/* @brief Means that entity doesn't exists. */
		inline constexpr EntityID kNullEntity = MaxValue<EntityID>( );

		template<detail::cComponent tType>
		struct EcsTraits;

	} // namespace lum::ecs

} // namespace lum