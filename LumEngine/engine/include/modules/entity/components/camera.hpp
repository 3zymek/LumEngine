//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Camera component for the ECS.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"

namespace lum {

    /* @brief ECS component representing a camera.
    * Stores projection and view parameters used for rendering.
    */
    struct CCamera : public Component {

        /* @brief Vertical field of view in degrees. */
        float32 mFov = 0;

        /* @brief Near clipping plane distance. */
        float32 mNear = 0;

        /* @brief Far clipping plane distance. */
        float32 mFar = 0;

        /* @brief Yaw angle in degrees, represents horizontal rotation. */
        float32 mYaw = -90.0f;

        /* @brief Pitch angle in degrees, represents vertical rotation. */
        float32 mPitch = 0.0f;

        /* @brief Point in world space the camera looks at. */
        glm::vec3 mTarget = glm::vec3(0.0f);

        /* @brief Up vector defining camera orientation. */
        glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);

    };

} // namespace lum