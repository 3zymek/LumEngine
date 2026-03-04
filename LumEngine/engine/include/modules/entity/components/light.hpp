//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Lights component for the ECS.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"

namespace lum {

    /* @brief Point light component — emits light in all directions from a single position.
    * Attenuates linearly based on distance and radius.
    */
    struct CPointLight : public Component {
        float32 mIntensity = 1.f;              // Light intensity in linear space
        float32 mRadius = 10.f;                // Maximum range of the light
        glm::vec3 mColor = { 1.f, 1.f, 1.f }; // Light color in linear RGB
    };

    /* @brief Spot light component — emits light in a cone shape from a single position.
    * @todo Not yet implemented.
    */
    struct CSpotLight : public Component {

    };

} // namespace lum