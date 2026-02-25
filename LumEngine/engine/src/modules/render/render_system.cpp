#pragma once

#include "render/render_system.hpp"

#include "render/renderer.hpp"
#include "entity/ecs_manager.hpp"

#include "entity/components/transform.hpp"
#include "entity/components/material.hpp"
#include "entity/components/mesh.hpp"
#include "entity/components/render.hpp"
#include "entity/components/name.hpp"
#include "entity/components/camera.hpp"

#include "platform/input_common.hpp"
#include "platform/key_codes.hpp"

#include "platform/window.hpp"

namespace lum {

	namespace render {

		void RenderSystem::Initialize(render::Renderer* renderer) {
		
			mRenderer = renderer;
		
		}

		void RenderSystem::Update(ecs::MEntityManager* entityMgr, Window* window) {

			float32 aspect = (float32)window->GetWidth() / (float32)window->GetHeight();

			entityMgr->Each<CCamera, CTransform>(
				[&](CCamera& camera, CTransform& transform) {
					FCameraData data;
					data.mPosition = transform.mPosition;
					data.mProjection = glm::perspective(glm::radians(camera.mFov), aspect, camera.mNear, camera.mFar);
					data.mView = glm::lookAt(transform.mPosition, camera.mTarget, camera.mUp);

					glm::vec3 forward = glm::normalize(camera.mTarget - transform.mPosition);
					glm::vec3 right = glm::normalize(glm::cross(forward, camera.mUp));

					float32 speed = 5.0f * 10.f;
					
					if (input::KeyPressed(input::Key::W)) transform.mPosition += forward * speed;
					if (input::KeyPressed(input::Key::S)) transform.mPosition -= forward * speed;
					if (input::KeyPressed(input::Key::A)) transform.mPosition -= right * speed;
					if (input::KeyPressed(input::Key::D)) transform.mPosition += right * speed;

					// mouse look
					glm::vec2 delta = input->GetMouseDelta();
					mYaw += delta.x * 0.1f;
					mPitch -= delta.y * 0.1f;
					mPitch = glm::clamp(mPitch, -89.0f, 89.0f);

					glm::vec3 direction;
					direction.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
					direction.y = sin(glm::radians(mPitch));
					direction.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

					camera.mTarget = transform.mPosition + glm::normalize(direction);

					mRenderer->UpdateCamera(data);
				}
			);

			entityMgr->Each<CRender, CTransform, CMaterial, CStaticMesh>(
				[&](CRender& render, CTransform& transform, CMaterial& material, CStaticMesh& mesh)
				{
					if (!render.bVisible) return;
					render::Object obj;
					obj.mMaterial = material.mMat;
					obj.mStaticMesh = mesh.mMesh;
					obj.mTransform = transform;
					mRenderer->Draw(obj);
				});
			
		}

	} // namespace lum::render

} // namespace lum