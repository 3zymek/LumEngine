//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Lights component for the ECS.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"

namespace lum {

    /* @brief Directional light component — simulates a distant light source (e.g. sun).
    * Affects all objects equally regardless of their position in the scene.
    */
    struct CDirectionalLight : Component {
        glm::vec3 mDirection = { 1.f, 0.f, 0.f }; // Direction the light is pointing
        float32 mIntensity = 1.f;                  // Light intensity in linear space
        glm::vec3 mColor = { 1.f, 1.f, 1.f };     // Light color in linear RGB
    };

    /* @brief Point light component — emits light in all directions from a single position.
    * Attenuates linearly based on distance and radius.
    */
    struct CPointLight : Component {
        float32 mIntensity = 1.f;              // Light intensity in linear space
        float32 mRadius = 10.f;                // Maximum range of the light
        glm::vec3 mColor = { 1.f, 1.f, 1.f }; // Light color in linear RGB
    };

    /* @brief Spot light component — emits light in a cone shape from a single position.
    * @todo Not yet implemented.
    */
    struct CSpotLight : Component {

    };

} // namespace lum