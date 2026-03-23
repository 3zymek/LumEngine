#pragma once

#include "engine.hpp"

namespace lum {

	class EditorCamera {
	public:

		void Initialize( ev::EventBus& bus ) {

			bus.SubscribePermanently< EWindowResized>(
				[&]( const EWindowResized& ev ) {

					mAspectRatio = ( float64 ) ev.mWidth / ( float64 ) ev.mHeight;

				} );

		}

		void Update( ) {



		}


	private:

		float32 mMovementSpeed = 0.5f;

		float32 mYaw = 0.0f;
		float32 mPitch = 0.0f;
		float32 mLastX = 0.0f;
		float32 mLastY = 0.0f;

		float64 mAspectRatio = 90.0f;

		float32 mFov = 90.0f;
		float32 mNearPlane = 0.1f;
		float32 mFarPlane = 1000.0f;

		glm::vec3 mPosition = glm::vec3( 0.0f );
		glm::vec3 mUp = glm::vec3( 0.0f, 1.0f, 0.0f );
		glm::vec3 mTarget = glm::vec3( 1.0f );


	};


}