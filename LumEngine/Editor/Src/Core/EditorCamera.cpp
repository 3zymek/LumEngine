#include "Core/EditorCamera.hpp"

#include "Core/Math/MathUtils.hpp"
#include "Modules/Platform/Input/Input.hpp"
#include "Modules/Rendering/Renderer.hpp"

namespace lum::editor {

	void EditorCamera::Update(float64 deltaTime, Renderer& renderer) {
		const bool looking = input::KeyPressed(Key::MouseRight);
		if (looking) {
			const Vector2 mousePosition = input::GetMousePosition();
			if (m_HasLastMousePosition) {
				const Vector2 mouseDelta = mousePosition - m_LastMousePosition;
				m_Yaw += mouseDelta.m_X * m_MouseSensitivity;
				m_Pitch = Clamp(m_Pitch - mouseDelta.m_Y * m_MouseSensitivity, -89.0f, 89.0f);
			}
			m_LastMousePosition = mousePosition;
			m_HasLastMousePosition = true;
		} else {
			m_HasLastMousePosition = false;
		}

		const float32 yaw = Radians(m_Yaw);
		const float32 pitch = Radians(m_Pitch);
		const Vector3 worldUp{ 0.0f, 1.0f, 0.0f };
		const Vector3 forward = Normalize(Vector3{
			cos(yaw) * cos(pitch),
			sin(pitch),
			sin(yaw) * cos(pitch)
		});
		const Vector3 right = Normalize(Cross(forward, worldUp));
		const Vector3 up = Normalize(Cross(right, forward));

		const float32 moveDistance = m_MovementSpeed * static_cast<float32>(deltaTime);
		if (input::KeyPressed(Key::W)) m_Position += forward * moveDistance;
		if (input::KeyPressed(Key::S)) m_Position -= forward * moveDistance;
		if (input::KeyPressed(Key::D)) m_Position += right * moveDistance;
		if (input::KeyPressed(Key::A)) m_Position -= right * moveDistance;
		if (input::KeyPressed(Key::Space)) m_Position += worldUp * moveDistance;
		if (input::KeyPressed(Key::LeftControl)) m_Position -= worldUp * moveDistance;

		CameraData data{};
		data.m_Position = m_Position;
		data.m_Front = forward;
		data.m_Right = right;
		data.m_Up = up;
		data.m_Fov = 60.0f;
		renderer.SubmitCameraData(data);
	}

} // namespace lum::editor
