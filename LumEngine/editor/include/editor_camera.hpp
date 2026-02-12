#pragma once
#include "core/core_pch.hpp"
#include "glad/glad.h"
#include "glfw3.h"

namespace lum {
	namespace editor {

		class Camera {
		public:
			Camera(Window* window) : mWindow(window) {
				mAspectRatio = static_cast<float>(window->get_width()) / static_cast<float>(window->get_height());
				glm::vec2 mousePos = input::GetMousePos();
				mLastX = mousePos.x;
				mLastY = mousePos.y;
			}

			void Update() {
				UpdateDirection();
				UpdateMovement();
				UpdateMatrices();
			}

			
			const math::Mat4& GetView() const { return mView; }
			const math::Mat4& GetProjection() const { return mProjection; }
			const math::Vec3& GetPosition() const { return mPosition; }
			const math::Vec3& GetFront() const { return mFront; }

			float GetMoveSpeed() const { return mMoveSpeed; }
			void SetMoveSpeed(float speed) { mMoveSpeed = speed; }

		private:
			void UpdateDirection() {
				math::Vec2 mousePos = input::GetMousePos();

				if (!mMouseLocked) {
					mLastX = mousePos.x;
					mLastY = mousePos.y;
					return;
				}

				float xOffset = mousePos.x - mLastX;
				float yOffset = mLastY - mousePos.y;

				mLastX = mousePos.x;
				mLastY = mousePos.y;

				xOffset *= mSensitivity;
				yOffset *= mSensitivity;

				mYaw += xOffset;
				mPitch += yOffset;

				mPitch = glm::clamp(mPitch, -89.0f, 89.0f);

				glm::vec3 direction;
				direction.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
				direction.y = sin(glm::radians(mPitch));
				direction.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

				mFront = glm::normalize(direction);
				mRight = glm::normalize(glm::cross(mFront, glm::vec3(0, 1, 0)));
				mUp = glm::normalize(glm::cross(mRight, mFront));
			}

			void UpdateMovement() {
				if (input::KeyPressed(input::Key::W)) {
					mPosition += mFront * mMoveSpeed;
				}
				if (input::KeyPressed(input::Key::S)) {
					mPosition -= mFront * mMoveSpeed;
				}
				if (input::KeyPressed(input::Key::A)) {
					mPosition -= mRight * mMoveSpeed;
				}
				if (input::KeyPressed(input::Key::D)) {
					mPosition += mRight * mMoveSpeed;
				}

				if (input::KeyPressedOnce(input::Key::LEFT_CONTROL)) {
					mMouseLocked = !mMouseLocked;
					auto* nativeWindow = static_cast<GLFWwindow*>(mWindow->get_native_window());
					int cursorMode = mMouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
					glfwSetInputMode(nativeWindow, GLFW_CURSOR, cursorMode);
				}
			}

			void UpdateMatrices() {
				mAspectRatio = static_cast<float>(mWindow->get_width()) / static_cast<float>(mWindow->get_height());
				mView = glm::lookAt(mPosition, mPosition + mFront, mUp);
				mProjection = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearPlane, mFarPlane);
			}

		private:
			Window* mWindow;

			math::Mat4 mView{ 1.0f };
			math::Mat4 mProjection{ 1.0f };

			math::Vec3 mPosition{ 0.0f, 0.0f, 0.0f };
			math::Vec3 mFront{ 0.0f, 0.0f, -1.0f };
			math::Vec3 mUp{ 0.0f, 1.0f, 0.0f };
			math::Vec3 mRight{ 1.0f, 0.0f, 0.0f };

			float32 mYaw = 0.0f;
			float32 mPitch = 0.0f;
			float32 mFOV = 45.0f;
			float32 mNearPlane = 0.1f;
			float32 mFarPlane = 1000.0f;
			float32 mSensitivity = 0.1f;
			float32 mMoveSpeed = 0.07f;
			float32 mAspectRatio = 1.0f;

			float32 mLastX = 0.0f;
			float32 mLastY = 0.0f;
			bool mMouseLocked = false;
		};

	}
}


