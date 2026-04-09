//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: ECS system handling camera and mesh rendering each frame.
//
//=============================================================================//

#include "render/render_system.hpp"
#include "render/renderer.hpp"

#include "entity/entity_manager.hpp"

#include "entity/components/transform.hpp"
#include "entity/components/material.hpp"
#include "entity/components/mesh.hpp"
#include "entity/components/render.hpp"
#include "entity/components/light.hpp"

#include "platform/window.hpp"

namespace lum {

	namespace render {

		void RenderSystem::Initialize( render::Renderer* renderer ) {

			LUM_ASSERT( renderer != nullptr, "Renderer is null" );

			mRenderer = renderer;

		}

		void RenderSystem::Update( ecs::MEntityManager* entityMgr, Window* window ) {

			entityMgr->Each<CRender, CTransform, CMaterialInstance, CStaticMesh>(
				[&]( CRender& render, CTransform& transform, CMaterialInstance& material, CStaticMesh& mesh ) {

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