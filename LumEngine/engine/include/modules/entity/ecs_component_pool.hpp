#pragma once

#include "entity/ecs_common.hpp"
#include "core/utils/sparse_set.hpp"

namespace lum::ecs { class MEntityManager; }

namespace lum::ecs::detail {

	struct BaseComponent { 
		virtual void Remove( EntityID enttiyID ) = 0;
		virtual ~BaseComponent( ) {}
	};

	template<Component tType>
	class ComponentPool : public BaseComponent {
	public:

		friend class MEntityManager;

		tType& Add( EntityID entityID, tType component ) {

			if (!Has(entityID))
				mComponents.Append(component, entityID);
			return mComponents[entityID];

		}
		tType* Get( EntityID entityID ) {
			return mComponents.Get(entityID);
		}
		void Remove( EntityID entityID ) override {
			mComponents.Remove( entityID );
		}
		bool Has( EntityID entityID ) {
			return mComponents.Contains( entityID );
		}

	protected:

		cstd::SparseSet<EntityID, tType> mComponents { limits::gMaxEntity };

	};
}