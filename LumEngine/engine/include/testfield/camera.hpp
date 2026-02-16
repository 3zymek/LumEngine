#pragma once

#include "core/core_common.hpp"

namespace lum {

    struct Window;

    class Camera {
    public:

        Camera(Window* wind) : m_window(wind) { Init(wind); }

        void Update() {

            RecalculateDirection();
            RecalculateMovement();
            RecalculateMVP();

        }

        glm::mat4 view = glm::mat4(1.f);
        glm::mat4 projection = glm::mat4(1.f);

        glm::vec3 position = { 0,0,0 };

        glm::vec3 front = { 0,0,-1 };
        glm::vec3 up = { 0,1,0 };
        glm::vec3 right = { 1,0,0 };
        Window* m_window = nullptr;
        float32 move_speed = 0.07f;

    private:

        void RecalculateDirection();

        void RecalculateMovement();

        void RecalculateMVP();

        void Init(Window* wind);

        float32 aspect_ratio = 0;
        float32 lastX = 0;
        float32 lastY = 0;
        float32 yaw = 0, pitch = 0;
        float32 fov = 45.f;
        float32 min_plane = 0.1f;
        float32 max_plane = 1000.f;
        float32 sensivity = 0.1f;
        bool mouse_locked = false;

        bool saveMousePos = false;
        bool mouseLocked = true;

    };


}