#pragma once

#include "EditorPch.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Render/RenderCommon.hpp"

namespace lum::editor {

	class EditorCamera {
	public:

		render::RenderCamera Update( float64 delta, bool unlocked = false ) {

			Vector3 forward = Normalize( mTarget - mPosition );
			Vector3 right = Normalize( Cross( forward, Vector3( 0, 1, 0 ) ) );
			Vector3 up = Normalize( Cross( right, forward ) );

			static bool sInitialized = false;
			static Vector2 sLastPos = Vector2( 0.0f, 0.0f );

			QPointF globalPosF = QCursor::pos( );
			
			Vector2 currentPos = Vector2( globalPosF.x( ), globalPosF.y( ) );

			if (!sInitialized) {
				sLastPos = currentPos;
				sInitialized = true;
			}

			if (unlocked) {

				float32 moveSpeed = mMovementSpeed * delta;

				

			}




		}

	private:

		float32 mMovementSpeed = 13.0f;
		float32 mSensivity = 0.1f;

		float32 m_Yaw = -90.0f;
		float32 mPitch = 0.0f;

		float32 mAspectRatio = 16.0f / 9.0f;

		float32 mFov = 90.0f;
		float32 mNearPlane = 0.1f;
		float32 mFarPlane = 1000.0f;

		Vector3 mPosition = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3 mUp = Vector3( 0.0f, 1.0f, 0.0f );
		Vector3 mTarget = Vector3( 1.0f, 0.0f, 0.0f );

	};

}