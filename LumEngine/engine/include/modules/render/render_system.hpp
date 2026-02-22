#pragma once

#include "core/core_common.hpp"
#include "render/renderer.hpp"
#include "entity/ecs_manager.hpp"
#include "render/texture_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/material_manager.hpp"
#include "render/render_system.hpp"

namespace lum {

	struct CRender {
		LUM_COMPONENT_TAG;
	};

	struct CMaterial {
		LUM_COMPONENT_TAG;
		FMaterialInstance mInstance;
	};
	struct CStaticMesh {
		LUM_COMPONENT_TAG;
		StaticMeshHandle mInstance;
	};

	namespace render {

		struct RenderSystem {
		public:

			RenderSystem( ) {}

			void Initialize( ecs::MEntityManager* entMgr, render::Renderer* renderer ) {
				mEntityManager = entMgr;
				mRenderer = renderer;
			}

			void Update() {

				mEntityManager->Each<CRender, CTransform, CMaterial, CStaticMesh>(
					[&](CRender& render, CTransform& transform, CMaterial& material, CStaticMesh& mesh)
					{
						render::Object obj;
						obj.mMaterial = material.mInstance;
						obj.mStaticMesh = mesh.mInstance;
						obj.mTransform = transform;
						mRenderer->Draw(obj);
					});

			}

		private:

			ecs::MEntityManager* mEntityManager = nullptr;
			render::Renderer* mRenderer = nullptr;

		};

	} // namespace lum::render

} // namespace lum