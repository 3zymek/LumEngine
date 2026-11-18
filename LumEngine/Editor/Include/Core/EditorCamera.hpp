#pragma once

#include "Core/CoreTypes.hpp"
#include "Core/Math/Vec2.hpp"
#include "Core/Math/Vec3.hpp"

namespace lum {
	class Renderer;
}

namespace lum::editor {

	class EditorCamera {
	public:
		// Hold the right mouse button to look around and use WASD to move.
		void Update(float64 deltaTime, Renderer& renderer);

		void SetAspectRatio(float32 ratio) noexcept { m_AspectRatio = ratio; }

	private:
		float32 m_MovementSpeed = 8.0f;
		float32 m_MouseSensitivity = 0.12f;
		float32 m_Yaw = -132.7f;
		float32 m_Pitch = -23.0f;
		float32 m_AspectRatio = 16.0f / 9.0f;
		Vector3 m_Position{ 8.0f, 5.0f, 8.0f };
		Vector2 m_LastMousePosition{};
		bool m_HasLastMousePosition = false;
	};

} // namespace lum::editor
