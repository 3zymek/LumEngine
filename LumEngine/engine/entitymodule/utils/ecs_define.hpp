#pragma once
#include "utils/e_define.hpp"
#include <type_traits>

namespace ecs::detail {

#define ComponentTag \
        static constexpr bool isComponent = true;

    template<typename T>
    concept Component =
        requires { T::isComponent; }&&
    std::is_trivially_copyable_v<T>&&
    std::is_trivially_destructible_v<T>;

    using EntityID = uint32_t;
    using EntityTypeID = uint32_t;

    constexpr unsigned int MAX_POOL_CAPACITY = 10000;

    struct GenerateEntityID {
        static EntityID Get() {
            static EntityID globalID = 0;
            return globalID++;
        }
    private:
        GenerateEntityID() {}
    };


    struct EntityTypeIndex {
        template<class EntityType>
        static EntityTypeID get() {
            static EntityTypeID id = count()++;
            return id;
        }
    private:
        static EntityTypeID& count() {
            static EntityTypeID globalID = 0;
            return globalID;
        }
    };
}
