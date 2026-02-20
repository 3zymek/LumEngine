#pragma once

#include "entity/ecs_common.hpp"
#include "core/utils/sparse_set.hpp"

namespace lum::ecs::detail {
	struct BaseComponent {};

	template<Component tType>
	class ComponentPool : public BaseComponent {
	public:

		tType* Add(EntityID entityID, tType component = {}) {
			mComponents.Append(tType{}, entityID);
			return mComponents.GetAt(entityID);
		}
		void Remove(EntityID entityID) {
			mComponents.Remove(entityID);
		}
		bool Has(EntityID entityID) {
			return mComponents.Contains(entityID);
		}


	private:

		cstd::SparseSet<uint32, tType> mComponents { limits::gMaxEcsPoolCapacity };

	};
}