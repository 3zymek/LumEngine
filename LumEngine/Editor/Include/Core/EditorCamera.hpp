#pragma once

#include "EditorPch.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Render/RenderCommon.hpp"

namespace lum::editor {

	class EditorCamera {
	public:

		enum class Direction {
			Forward,
			Backward,
			Right,
			Left
		};

		void Move( Direction dir ) {
			float32 moveSpeed = (m_MovementSpeed * m_DeltaTime);
			switch (dir) {
				case Direction::Forward:
				{
					m_Position += (m_Forward * moveSpeed);
					break;
				}
				case Direction::Backward:
				{
					m_Position -= (m_Forward * moveSpeed);
					break;
				}
				case Direction::Right:
				{
					m_Position += (m_Right * moveSpeed);
					break;
				}
				case Direction::Left:
				{
					m_Position -= (m_Right * moveSpeed);
					break;
				}
				default: break;
			}
		}

		render::RenderCamera Update( float64 delta, bool unlocked = false ) {

			m_Forward = Normalize( m_Target - m_Position );
			m_Right = Normalize( Cross( m_Forward, Vector3( 0.0f, 1.0f, 0.0f ) ) );
			Vector3 up = Normalize( Cross( m_Right, m_Forward ) );

			static bool s_Initialized = false;
			static Vector2 s_LastPos = Vector2( 0.0f, 0.0f );

			QPoint pos = QCursor::pos( );

			Vector2 currentPos = Vector2(
				SafeCast<float32>( pos.x( ) ),
				SafeCast<float32>( pos.y( ) )
			);

			if (!s_Initialized) {
				s_LastPos = currentPos;
				s_Initialized = true;
			}

			if (unlocked) {

				m_DeltaTime = delta;

				Vector2 deltaPos = currentPos - s_LastPos;
				s_LastPos = currentPos;

				m_Yaw += deltaPos.m_X * m_Sensivity;
				m_Pitch -= deltaPos.m_Y * m_Sensivity;
				m_Pitch = Clamp( m_Pitch, -89.0f, 89.0f );

				Vector3 dir{};
				dir.m_X = cos( Radians( m_Yaw ) * cos( Radians( m_Pitch ) ) );
				dir.m_Y = sin( Radians( m_Pitch ) );
				dir.m_Z = sin( Radians( m_Yaw ) ) * cos( Radians( m_Pitch ) );
				m_Target = m_Position + Normalize( dir );
				m_Up = up;

			}
			else s_LastPos = currentPos;

			render::RenderCamera data{};
			data.m_Position = m_Position;
			data.m_Projection = Perspective( Radians( m_Fov ), m_AspectRatio, m_NearPlane, m_FarPlane );
			data.m_View = LookAt( m_Position, m_Target, m_Up );

			return data;

		}

		inline void SetAspectRatio( float32 ratio ) noexcept {
			m_AspectRatio = ratio;
		}

	private:

		float32 m_DeltaTime = 0.0f;

		float32 m_MovementSpeed = 13.0f;
		float32 m_Sensivity = 0.1f;

		float32 m_Yaw = -90.0f;
		float32 m_Pitch = 0.0f;

		float32 m_AspectRatio = 16.0f / 9.0f;

		float32 m_Fov = 90.0f;
		float32 m_NearPlane = 0.1f;
		float32 m_FarPlane = 1000.0f;

		Vector3 m_Position = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3 m_Up = Vector3( 0.0f, 1.0f, 0.0f );
		Vector3 m_Target = Vector3( 1.0f, 0.0f, 0.0f );

		Vector3 m_Forward = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3 m_Right = Vector3( 1.0f, 0.0f, 0.0f );

	};

} // namespace lum::editor