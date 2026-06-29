//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: ECS system handling camera and mesh rendering each frame.
//
//=============================================================================//

#include "render/render_system.hpp"
#include "render/renderer.hpp"

#include "entity/entity_manager.hpp"

#include "scene/scene_manager.hpp"

#include "entity/components/transform.hpp"
#include "entity/components/material.hpp"
#include "entity/components/mesh.hpp"
#include "entity/components/light.hpp"

#include "platform/window.hpp"

namespace lum {

	namespace render {

		void RenderSystem::Initialize( render::Renderer* renderer ) {

			LUM_ASSERT( renderer != nullptr, "Renderer is null" );
			mRenderer = renderer;

		}

		void RenderSystem::Update( Scene* scene, Window* window ) {

			auto isVisible = [&]( EntityID id ) { Entity* e = scene->GetEntity( id ); return e && e->IsVisible( ); };
		
			scene->mEntityMgr.EachWithID<CTransform, CMaterialInstance, CStaticMesh>(
				[&]( EntityID id, CTransform& transform, CMaterialInstance& material, CStaticMesh& mesh ) {

					if (!isVisible( id )) return;

					render::RenderInstance inst;
					inst.mMaterial = &material;
					inst.mStaticMesh = &mesh;
					inst.mTransform = &transform;

					mRenderer->Submit( inst );

				}
			);

			scene->mEntityMgr.EachWithID<CTransform, CPointLight>(
				[&]( EntityID id, CTransform& transform, CPointLight& light ) {

					if (!isVisible( id )) return;

					PointLight pointL;

					pointL.mPosition = transform.mPosition;
					pointL.mIntensity = light.mIntensity;
					pointL.mColor = light.mColor;
					pointL.mRadius = light.mRadius;

					mRenderer->AddPointLight( pointL );

				}
			);

			scene->mEntityMgr.EachWithID<CTransform, CSpotLight>(
				[&]( EntityID id, CTransform& transform, CSpotLight& light ) {

					if (!isVisible( id )) return;

					SpotLight spotL;

					spotL.mPosition = transform.mPosition;
					spotL.mIntensity = light.mIntensity;
					spotL.mColor = light.mColor;
					spotL.mRadius = light.mRadius;
					spotL.mDirection = Normalize( light.mDirection );
					spotL.mInnerCone = cos( Radians( light.mInnerAngle ) );
					spotL.mOuterCone = cos( Radians( light.mOuterCone ) );

					mRenderer->AddSpotLight( spotL );

				}
			);
		}

	} // namespace lum::render

} // namespace lum