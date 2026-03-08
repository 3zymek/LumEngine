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
        
        float32 mIntensity = 1.f; // Light intensity in linear space
        
        float32 mRadius = 10.f; // Maximum range of the light
        
        glm::vec3 mColor = { 1.f, 1.f, 1.f }; // Light color in linear RGB
    
    };

    /* @brief Spot light component — emits light in a cone shape from a single position. */
    struct CSpotLight : public Component {
        
        float32 mIntensity = 100.f;

        float32 mRadius = 10.f; // Maximum range of the light

        float32 mInnerConeAngle = 15.f;

        float32 mOuterConeAngle = 30.f;

        glm::vec3 mDirection = glm::vec3(0.f, -1.f, 0.f);

        glm::vec3 mColor = { 1.f, 1.f, 1.f }; // Light color in linear RGB

    };

} // namespace lum