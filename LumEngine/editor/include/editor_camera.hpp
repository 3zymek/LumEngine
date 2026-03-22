#pragma once

#include "engine.hpp"

namespace lum {

	class EditorCamera {
	public:

		void Initialize( ev::EventBus& bus ) {

			bus.SubscribePermanently< EWindowResized>(
				[&]( const EWindowResized& ev ) {

					mAspectRatio = ( float32 ) ev.mWidth / ( float32 ) ev.mHeight;

				} );

			bus.SubscribePermanently<EKeyPressed>(
				[&]( const EKeyPressed& ev ) {

					if (ev.mKey == input::Key::LEFT_CONTROL) {
						bLocked = !bLocked;
					}

				}
			);

		}

		render::FRenderCamera Update( float64 delta ) {

			glm::vec3 forward = glm::normalize( mTarget - mPosition );
			glm::vec3 right = glm::normalize( glm::cross( forward, glm::vec3( 0, 1, 0 ) ) );
			glm::vec3 up = glm::normalize( glm::cross( right, forward ) );

			static glm::vec2 sLastPos = input::GetMousePos( );

			if (!bLocked) {

				float32 moveSpeed = mMovementSpeed * delta;

				if (input::KeyPressed( input::Key::W )) mPosition += forward * moveSpeed;
				if (input::KeyPressed( input::Key::S )) mPosition -= forward * moveSpeed;
				if (input::KeyPressed( input::Key::A )) mPosition -= right * moveSpeed;
				if (input::KeyPressed( input::Key::D )) mPosition += right * moveSpeed;

				glm::vec2 currentPos = input::GetMousePos( );
				glm::vec2 deltaPos = currentPos - sLastPos;
				sLastPos = currentPos;

				mYaw += deltaPos.x * mSensivity;
				mPitch -= deltaPos.y * mSensivity;
				mPitch = glm::clamp( mPitch, -89.0f, 89.0f );

				glm::vec3 direction;
				direction.x = cos( glm::radians( mYaw ) ) * cos( glm::radians( mPitch ) );
				direction.y = sin( glm::radians( mPitch ) );
				direction.z = sin( glm::radians( mYaw ) ) * cos( glm::radians( mPitch ) );
				mTarget = mPosition + glm::normalize( direction );
				mUp = up;

			}

			render::FRenderCamera data;

			data.mPosition = mPosition;
			data.mProjection = glm::perspective( glm::radians( mFov ), mAspectRatio, mNearPlane, mFarPlane );
			data.mView = glm::lookAt( mPosition, mTarget, mUp );

			return data;

		}


	private:

		bool bLocked = false;

		float32 mMovementSpeed = 0.01f;
		float32 mSensivity = 0.1f;

		float32 mYaw = 0.0f;
		float32 mPitch = 0.0f;

		float32 mAspectRatio = 16.0f / 9.0f;

		float32 mFov = 90.0f;
		float32 mNearPlane = 0.1f;
		float32 mFarPlane = 1000.0f;

		glm::vec3 mPosition = glm::vec3( 0.0f );
		glm::vec3 mUp = glm::vec3( 0.0f, 1.0f, 0.0f );
		glm::vec3 mTarget = glm::vec3( 1.0f );


	};


}