//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: ECS system handling camera and mesh rendering each frame.
//
//=============================================================================//

#include "Render/RenderSystem.hpp"
#include "Render/Renderer.hpp"

#include "Entity/EntityManager.hpp"

#include "Scene/SceneManager.hpp"

#include "Entity/Components/Transform.hpp"
#include "Entity/Components/Material.hpp"
#include "Entity/Components/Mesh.hpp"
#include "Entity/Components/PointLight.hpp"
#include "Entity/Components/SpotLight.hpp"

#include "Platform/Window.hpp"

namespace lum {

	namespace render {

		void RenderSystem::Initialize( render::Renderer* renderer ) {

			LUM_ASSERT( renderer != nullptr, "Renderer is null" );
			m_Renderer = renderer;

		}

		void RenderSystem::Update( SceneInstance& scene ) {

			auto isVisible = [ & ]( EntityID id ) { auto e = scene.GetEntity( id ); return e && e->IsVisible( ); };

			scene.m_EntityMgr.EachWithID<CTransform, CMaterialInstance, CStaticMesh>(
				[ & ]( EntityID id, CTransform& transform, CMaterialInstance& material, CStaticMesh& mesh ) {

					if (!isVisible( id )) return;

					render::RenderInstance inst;
					inst.m_Material = &material;
					inst.m_StaticMesh = &mesh;
					inst.m_Transform = &transform;


					m_Renderer->SubmitInstance( inst );

				}
			);

			scene.m_EntityMgr.EachWithID<CTransform, CPointLight>(
				[ & ]( EntityID id, CTransform& transform, CPointLight& light ) {

					if (!isVisible( id )) return;

					PointLight pointL;

					pointL.m_Position = transform.m_Position;
					pointL.m_Intensity = light.m_Intensity;
					pointL.m_Color = light.m_Color;
					pointL.m_Radius = light.m_Radius;

					m_Renderer->AddPointLight( pointL );

				}
			);

			scene.m_EntityMgr.EachWithID<CTransform, CSpotLight>(
				[ & ]( EntityID id, CTransform& transform, CSpotLight& light ) {

					if (!isVisible( id )) return;

					SpotLight spotL;

					spotL.m_Position = transform.m_Position;
					spotL.m_Intensity = light.m_Intensity;
					spotL.m_Color = light.m_Color;
					spotL.m_Radius = light.m_Radius;
					spotL.m_Direction = Normalize( light.m_Direction );
					spotL.m_InnerCone = cos( Radians( light.m_InnerAngle ) );
					spotL.m_OuterCone = cos( Radians( light.m_OuterCone ) );

					m_Renderer->AddSpotLight( spotL );

				}
			);
		}

	} // namespace lum::render

} // namespace lum