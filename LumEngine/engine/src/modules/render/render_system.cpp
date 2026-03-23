#pragma once

#include "render/render_system.hpp"
#include "render/renderer.hpp"

#include "entity/entity_manager.hpp"
#include "entity/components/transform.hpp"
#include "entity/components/material.hpp"
#include "entity/components/mesh.hpp"
#include "entity/components/render.hpp"
#include "entity/components/name.hpp"
#include "entity/components/camera.hpp"
#include "entity/components/light.hpp"

#include "platform/input_common.hpp"
#include "platform/key_codes.hpp"
#include "platform/window.hpp"

#include "imgui.h"

namespace lum {

	namespace render {

		void RenderSystem::Initialize( render::Renderer* renderer ) {

			mRenderer = renderer;

		}

		void RenderSystem::Update( ecs::MEntityManager* entityMgr, Window* window ) {

			float32 width = ( float32 ) window->GetWidth( );
			float32 height = ( float32 ) window->GetHeight( );

			if (width == 0 || height == 0)
				return;

			float32 aspect = width / height;

			entityMgr->Each<CCamera, CTransform>(
				[&]( CCamera& camera, CTransform& transform ) {

					FRenderCamera data;

					data.mPosition = transform.mPosition;
					data.mProjection = glm::perspective( glm::radians( camera.mFov ), aspect, camera.mNear, camera.mFar );
					data.mView = glm::lookAt( transform.mPosition, camera.mTarget, camera.mUp );

					glm::vec3 forward = glm::normalize( camera.mTarget - transform.mPosition );
					glm::vec3 right = glm::normalize( glm::cross( forward, glm::vec3( 0, 1, 0 ) ) );
					glm::vec3 up = glm::normalize( glm::cross( right, forward ) );

					static bool     sCameraLocked = false;
					static glm::vec2 sLastMousePos = input::GetMousePos( );

					if (input::KeyPressedOnce( input::Key::LEFT_CONTROL ))
						sCameraLocked = !sCameraLocked;

					if (!sCameraLocked) {
						float32 speed = 5.0f * 0.1f;
						
						if (input::KeyPressed( input::Key::W )) transform.mPosition += forward * speed;
						if (input::KeyPressed( input::Key::S )) transform.mPosition -= forward * speed;
						if (input::KeyPressed( input::Key::A )) transform.mPosition -= right * speed;
						if (input::KeyPressed( input::Key::D )) transform.mPosition += right * speed;

						glm::vec2 currentPos = input::GetMousePos( );
						glm::vec2 delta = currentPos - sLastMousePos;
						sLastMousePos = currentPos;

						camera.mYaw += delta.x * 0.1f;
						camera.mPitch -= delta.y * 0.1f;
						camera.mPitch = glm::clamp( camera.mPitch, -89.0f, 89.0f );

					}
					else {
						sLastMousePos = input::GetMousePos( );
					}

					glm::vec3 direction;
					direction.x = cos( glm::radians( camera.mYaw ) ) * cos( glm::radians( camera.mPitch ) );
					direction.y = sin( glm::radians( camera.mPitch ) );
					direction.z = sin( glm::radians( camera.mYaw ) ) * cos( glm::radians( camera.mPitch ) );
					camera.mTarget = transform.mPosition + glm::normalize( direction );
					camera.mUp = up;

					mRenderer->UpdateCamera( data );
				}
			);

			entityMgr->Each<CRender, CTransform, CMaterial, CStaticMesh>(
				[&]( CRender& render, CTransform& transform, CMaterial& material, CStaticMesh& mesh ) {

					if (!render.bVisible) return;

					render::FRenderInstance inst;
					inst.mMaterial = &material;
					inst.mStaticMesh = &mesh;
					inst.mTransform = &transform;

					mRenderer->Submit( inst );

				}
			);

			entityMgr->Each<CTransform, CPointLight>(
				[&]( CTransform& transform, CPointLight& light ) {

					FPointLight pointL;

					pointL.mPosition = transform.mPosition;
					pointL.mIntensity = light.mIntensity;
					pointL.mColor = light.mColor;
					pointL.mRadius = light.mRadius;

					mRenderer->AddPointLight( pointL );

				}
			);

			entityMgr->Each<CTransform, CSpotLight>(
				[&]( CTransform& transform, CSpotLight& light ) {

					FSpotLight spotL;

					spotL.mPosition = transform.mPosition;
					spotL.mIntensity = light.mIntensity;
					spotL.mColor = light.mColor;
					spotL.mRadius = light.mRadius;
					spotL.mDirection = glm::normalize( light.mDirection );
					spotL.mInnerCone = glm::cos( glm::radians( light.mInnerConeAngle ) );
					spotL.mOuterCone = glm::cos( glm::radians( light.mOuterConeAngle ) );

					mRenderer->AddSpotLight( spotL );

				}
			);
		}

	} // namespace lum::render

} // namespace lum