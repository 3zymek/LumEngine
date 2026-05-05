#pragma once

#include "event/events/window_events.hpp"

namespace lum::editor {

	class EditorCamera {
	public:

		void Initialize( ev::EventBus& bus ) {

			bus.SubscribePermanently<EKeyPressed>(
				[&]( const EKeyPressed& ev ) {
					if (ev.mKey == input::Key::LEFT_CONTROL) {
						bLocked = !bLocked;
					}
				}
			);
		}

		render::FRenderCamera Update( float64 delta ) {

			Vector3 forward = Normalize( mTarget - mPosition );
			Vector3 right = Normalize( Cross( forward, Vector3( 0, 1, 0 ) ) );
			Vector3 up = Normalize( Cross( right, forward ) );

			static Vector2 sLastPos = input::GetMousePos( );

			if (!bLocked) {

				float32 moveSpeed = mMovementSpeed * delta;

				if (input::KeyPressed( input::Key::W )) mPosition += forward * moveSpeed;
				if (input::KeyPressed( input::Key::S )) mPosition -= forward * moveSpeed;
				if (input::KeyPressed( input::Key::A )) mPosition -= right * moveSpeed;
				if (input::KeyPressed( input::Key::D )) mPosition += right * moveSpeed;

				Vector2 currentPos = input::GetMousePos( );
				Vector2 deltaPos = currentPos - sLastPos;
				sLastPos = currentPos;

				mYaw += deltaPos.x * mSensivity;
				mPitch -= deltaPos.y * mSensivity;
				mPitch = std::clamp( mPitch, -89.0f, 89.0f );

				Vector3 direction;
				direction.x = cos( Radians( mYaw ) ) * cos( Radians( mPitch ) );
				direction.y = sin( Radians( mPitch ) );
				direction.z = sin( Radians( mYaw ) ) * cos( Radians( mPitch ) );
				mTarget = mPosition + Normalize( direction );
				mUp = up;

			}
			else sLastPos = input::GetMousePos();

			render::FRenderCamera data;

			data.mPosition = mPosition;
			data.mProjection = Perspective( Radians( mFov ), mAspectRatio, mNearPlane, mFarPlane );
			data.mView = LookAt( mPosition, mTarget, mUp );

			return data;

		}

		inline void SetAspectRatio( float32 ratio ) noexcept { mAspectRatio = ratio; }


	private:

		bool bLocked = false;

		float32 mMovementSpeed = 13.0f;
		float32 mSensivity = 0.1f;

		float32 mYaw = 0.0f;
		float32 mPitch = 0.0f;

		float32 mAspectRatio = 16.0f / 9.0f;

		float32 mFov = 90.0f;
		float32 mNearPlane = 0.1f;
		float32 mFarPlane = 1000.0f;

		Vector3 mPosition;
		Vector3 mUp;
		Vector3 mTarget;


	};


}