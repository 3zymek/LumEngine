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
					mRenderer->UpdateCamera(data);
				}
			);

			entityMgr->Each<CRender, CTransform, CMaterial, CStaticMesh>(
				[&](CRender& render, CTransform& transform, CMaterial& material, CStaticMesh& mesh)
				{
					render::Object obj;
					obj.mMaterial = material.mMat;
					obj.mStaticMesh = mesh.mMesh;
					obj.mTransform = transform;
					mRenderer->Draw(obj);
				});

		}

	} // namespace lum::render

} // namespace lum