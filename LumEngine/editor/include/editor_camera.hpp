#pragma once
#include "core/core_pch.hpp"
#include "render/essentials/uniform_buffer.hpp"
#include "glad/glad.h"
#include "glfw3.h"
namespace lum {
	namespace editor {

		class EditorCamera {
		public:

			EditorCamera(float aspect_rat) : aspect_ratio(aspect_rat) {}

			void Update( ) {

				HandleInput( );
				RecalculateCameraDir( );

				ubo.Update(glm::value_ptr(projection), sizeof(glm::mat4), 0);
				ubo.Update(glm::value_ptr(view), sizeof(glm::mat4), sizeof(glm::mat4));

			}

			glm::vec3 GetPosition() const {
				return position;
			}

			glm::vec3 GetForward() const {
				return front;
			}

			glm::vec3 GetUp() const {
				return up;
			}

		private:

			void RecalculateCameraDir( ) {

				glm::vec2 mouse_pos = input::GetMousePos();

				if (!mouse_locked) {
					lastX = mouse_pos.x;
					lastY = mouse_pos.y;
					return;
				}

				double xoffset = mouse_pos.x - lastX;
				double yoffset = lastY - mouse_pos.y;

				xoffset *= sensivity;
				yoffset *= sensivity;

				yaw += xoffset;
				pitch += yoffset;

				lastX = mouse_pos.x;
				lastY = mouse_pos.y;

				if (pitch >= 89.f) pitch = 89.f;
				if (pitch <= -89.f) pitch = -89.f;

				glm::vec3 camDir;
				camDir.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
				camDir.y = glm::sin(glm::radians(pitch));
				camDir.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

				front	= glm::normalize(camDir);
				right	= glm::normalize(glm::cross(front, {0, 1, 0}));
				up		= glm::normalize(glm::cross(right, front));

				view = glm::lookAt(position, position + front, up);
				projection = glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);

			}

			void HandleInput( ) {

				if (input::KeyPressed(input::Key::W)) {
					position += move_speed * front;
				}
				if (input::KeyPressed(input::Key::S)) {
					position -= move_speed * front;
				}
				if (input::KeyPressed(input::Key::A)) {
					position -= move_speed * right;
				}
				if (input::KeyPressed(input::Key::D)) {
					position += move_speed * right;
				}

			}

			render::UniformBuffer ubo { 0, 2 * sizeof(glm::mat4) };
			glm::vec3 position		= { 0.f, 0.f, 0.f };
			glm::vec3 up			= { 0.f, 1.f, 0.f };
			glm::vec3 right			= { 1.f, 0.f, 0.f };
			glm::vec3 front			= { 0.f, 0.f, -1.f };
			glm::mat4 view			= glm::mat4(1);
			glm::mat4 projection	= glm::mat4(1);
			float mouse_locked = true;
			float move_speed = 0.4f;
			float sensivity = 0.1f;
			float lastX = 0, lastY = 0;
			float yaw	= -90.f;
			float pitch = 0.f;
			float fov = 40.f;
			float aspect_ratio;
			float far_plane = 10000.f;
			float near_plane = 0.1f;

		};
	}
}


