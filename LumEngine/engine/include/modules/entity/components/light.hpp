//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Lights component for the ECS.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"

namespace lum {

    struct CDirectionalLight : Component {

        glm::vec3 mDirection = { 1.f, 0.f, 0.f };
        float32 mIntensity = 1.f;
        glm::vec3 mColor = { 1.f, 1.f, 1.f };

    };

    struct CPointLight : Component {

        float32 mIntensity = 1.f;
        float32 mRadius = 10.f;

    };

    struct CSpotLight : Component {

        

    };

} // namespace lum