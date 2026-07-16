#pragma once

#include "Event/Events/WindowEvents.hpp"

namespace lum::editor {

	class EditorCamera {
	public:

		render::RenderCamera Update( float64 delta, bool moveMouse = true ) {

			Vector3 forward = Normalize( mTarget - mPosition );
			Vector3 right = Normalize( Cross( forward, Vector3( 0, 1, 0 ) ) );
			Vector3 up = Normalize( Cross( right, forward ) );

			static bool sbInitialized = false;
			static Vector2 sLastPos;

			Vector2 currentPos = input::GetMousePos( );

			if (!sbInitialized) {
				sLastPos = currentPos;
				sbInitialized = true;
			}

			if (moveMouse) {

				float32 moveSpeed = mMovementSpeed * delta;

				if (input::KeyPressed( input::Key::W )) mPosition += forward * moveSpeed;
				if (input::KeyPressed( input::Key::S )) mPosition -= forward * moveSpeed;
				if (input::KeyPressed( input::Key::A )) mPosition -= right * moveSpeed;
				if (input::KeyPressed( input::Key::D )) mPosition += right * moveSpeed;

				Vector2 currentPos = input::GetMousePos( );
				Vector2 deltaPos = currentPos - sLastPos;
				sLastPos = currentPos;

				mYaw += deltaPos.mX * mSensivity;
				mPitch -= deltaPos.mY * mSensivity;
				mPitch = std::clamp( mPitch, -89.0f, 89.0f );

				Vector3 direction;
				direction.mX = cos( Radians( mYaw ) ) * cos( Radians( mPitch ) );
				direction.mY = sin( Radians( mPitch ) );
				direction.mZ = sin( Radians( mYaw ) ) * cos( Radians( mPitch ) );
				mTarget = mPosition + Normalize( direction );
				mUp = up;

			}
			else sLastPos = input::GetMousePos( );

			render::RenderCamera data;

			data.mPosition = mPosition;
			data.mProjection = Perspective( Radians( mFov ), mAspectRatio, mNearPlane, mFarPlane );
			data.mView = LookAt( mPosition, mTarget, mUp );

			return data;

		}

		inline void SetAspectRatio( float32 ratio ) noexcept { mAspectRatio = ratio; }


	private:

		float32 mMovementSpeed = 13.0f;
		float32 mSensivity = 0.1f;

		float32 mYaw = -90.0f;
		float32 mPitch = 0.0f;

		float32 mAspectRatio = 16.0f / 9.0f;

		float32 mFov = 90.0f;
		float32 mNearPlane = 0.1f;
		float32 mFarPlane = 1000.0f;

		Vector3 mPosition = Vector3( 0.0f, 0.0f, 3.0f );
		Vector3 mUp = Vector3( 0.0f, 1.0f, 0.0f );
		Vector3 mTarget;

	};


}