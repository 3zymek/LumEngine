#pragma once

#include "core/core_common.hpp"
#include "window_context/window.hpp"

namespace lum {

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

        void RecalculateDirection() {

            glm::vec2 mouse_pos = input::GetMousePos();

            if (!mouse_locked) {
                lastX = mouse_pos.x;
                lastY = mouse_pos.y;
                return;
            }

            float xpos = mouse_pos.x - lastX;
            float ypos = lastY - mouse_pos.y;

            lastX = mouse_pos.x;
            lastY = mouse_pos.y;

            xpos *= sensivity;
            ypos *= sensivity;

            yaw += xpos;
            pitch += ypos;

            if (pitch >= 89.f) pitch = 89.f;
            if (pitch <= -89.f) pitch = -89.f;

            glm::vec3 new_view = { 0,0,0 };
            new_view.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            new_view.y = sin(glm::radians(pitch));
            new_view.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

            front = glm::normalize(new_view);
            right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
            up = glm::normalize(glm::cross(right, front));

        }

        void RecalculateMovement() {

            if (input::KeyPressed(input::Key::W)) {
                position += front * move_speed;
                //std::cout << "w";
            }
            if (input::KeyPressed(input::Key::S)) {
                position -= front * move_speed;
                //std::cout << "s";
            }
            if (input::KeyPressed(input::Key::A)) {
                position -= right * move_speed;
                //std::cout << "a";
            }
            if (input::KeyPressed(input::Key::D)) {
                position += right * move_speed;
                //std::cout << "d";
            }
            if (input::KeyPressedOnce(input::Key::LEFT_CONTROL)) {
                mouse_locked = !mouse_locked;
                if (mouse_locked) {
                    glfwSetInputMode(static_cast<GLFWwindow*>(m_window->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
                else glfwSetInputMode(static_cast<GLFWwindow*>(m_window->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }

        }

        void RecalculateMVP() {
            aspect_ratio = (float32)m_window->GetWidth() / (float32)m_window->GetHeight();
            view = glm::lookAt(position, front + position, up);
            projection = glm::perspective(glm::radians(fov), aspect_ratio, min_plane, max_plane);

        }

        void Init(Window* wind) {
            aspect_ratio = (double)wind->GetWidth() / (double)wind->GetHeight();
            glm::vec2 mouse_pos = input::GetMousePos();
            lastX = mouse_pos.x;
            lastY = mouse_pos.y;
        }

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