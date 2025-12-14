#pragma once
#include "core/details/e_define.hpp"
#include <type_traits>
#include <exception>
#include "core_defines.hpp"
namespace ecs {
    using EntityID = uint32_t;
}

namespace ecs::detail {

    class UniqueComponentAlreadyExists : std::exception {
        std::string msg;
    public:
        UniqueComponentAlreadyExists(const std::string& m) noexcept : msg(m) {}
        const char* what() const noexcept override {
            return msg.c_str();
        }
    };

    inline constexpr unsigned int MAX_POOL_CAPACITY         = settings::MAX_POOL_CAPACITY;
    inline constexpr unsigned int MAX_ENTITY_COUNT          = settings::MAX_ENTITY_COUNT;
    inline constexpr unsigned int MAX_COMPONENT_TYPES_COUNT = settings::MAX_COMPONENT_TYPES_COUNT;

#define LumComponentTag \
        static constexpr bool isComponent = true;

    struct UniqueComponent {};

    template<typename T>
    concept Component =
        requires { T::isComponent; }&&
    std::is_trivially_copyable_v<T> &&
    std::is_trivially_destructible_v<T>;

    using ComponentTypeID = uint32_t;

    struct GenerateEntityID {
        static EntityID Get() {
            static EntityID globalID = 0;
            return globalID++;
        }
    private:
        GenerateEntityID() {}
    };


    struct ComponentTypeIndex {
        template<class EntityType>
        static ComponentTypeID get() {
            static ComponentTypeID id = count()++;
            return id;
        }
    private:
        static ComponentTypeID& count() {
            static ComponentTypeID globalID = 0;
            return globalID;
        }
    };
}
