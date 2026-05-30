//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Entity and component primitives for the ECS.
//
//=============================================================================//
#pragma once
#include "core/core_common.hpp"

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

		/* @brief Means that entity doesn't exists. */
		inline constexpr EntityID kNullEntity = MaxValue<EntityID>( );

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

			/* @brief Returns the serialization name of the component type.
			*  Used for reading and writing component data to scene files.
			*/
			template<cComponent tType>
			StringView GetComponentParseName( );

			/* @brief Returns the human-readable display name shown in the editor. */
			template<cComponent tType>
			StringView GetComponentDisplayName( );

			/* @brief Returns the editor category this component belongs to (e.g. "Rendering", "Physics"). */
			template<cComponent tType>
			StringView GetComponentCategoryName( );

		} // namespace lum::ecs::detail

	} // namespace lum::ecs

} // namespace lum