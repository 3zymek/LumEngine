#pragma once

#include "Core/CoreTypes.hpp"
#include "Core/CoreDefines.hpp"

#include <atomic>
#include <concepts>

namespace lum {

	/* @brief Generates a unique numeric ID for a given type.
	* Each instantiation maintains its own atomic counter starting at tNullVal + 1.
	* @tparam tType  Type to generate IDs for.
	* @tparam tNullVal Sentinel null value; the counter starts one above this.
	*/
	template<typename tType, std::integral tIntegral = usize, tIntegral tNullVal = 0>
	struct LUM_API UniqueID {

		/* @brief Returns the next available unique ID.
		* @return Atomically incremented uint32 identifier.
		*/
		static tIntegral Get( ) {
			return s_Id.fetch_add( 1, std::memory_order_relaxed );
		}

	private:

		/* @brief Shared atomic counter for this type instantiation. */
		inline static std::atomic<tIntegral> s_Id = tNullVal + 1;

	};



	/* @brief Generates a unique compile-time stable ID per type.
	* Each distinct type receives a single ID assigned on first call.
	*/
	struct LUM_API TypeRegistry {

		/* @brief Returns the unique ID assigned to tType.
		* @tparam tType Type to retrieve the ID for.
		* @return Stable uint32 identifier for the given type.
		*/
		template<typename tType>
		static uint32 GetTypeId( ) {
			static uint32 id = s_Id.fetch_add( 1, std::memory_order_relaxed );
			return id;
		}

	private:

		/* @brief Global atomic counter shared across all type ID assignments. */
		inline static std::atomic<uint32> s_Id = 0;

	};

} // namespace lum