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