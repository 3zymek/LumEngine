#pragma once
#include "core/engine.hpp"
#include "editor_camera.hpp"
#include "entity/entity.hpp"
#include "entity/components/c_transform.hpp"
#include "render/components/c_mesh.hpp"
#include "audio/components/c_audio_listener.hpp"
namespace lum {
	namespace editor {

		class Editor { 
		public:

			Editor(core::Engine& eng) : engine(eng) {}

			void Update() {

				camera.Update();
				
			}

			void Run() {

				// shadery
				auto& shaders = engine.GetShaderManager();

				// mesh
				auto& mm = engine.GetMeshManager();
				std::vector<render::Vertex> vertices = {
					{ {1, 1, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0} },
					{ {-1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {0, 0} },
					{ {0, -1, 0}, {0, 0, 1}, {0, 0, 0}, {0, 0} }
				};
				std::vector<render::Index> indices = { 0, 1, 2 };

				// ecs
				Entity entity = engine.GetECSManager().CreateEntity();
				auto* transform = entity.AddComponent<ecs::components::TransformComponent>();
				auto* mesh = entity.AddComponent<ecs::components::MeshComponent>();
				mesh->handle = mm.CreateStaticMesh(vertices, indices);


				Entity e2 = engine.GetECSManager().CreateEntity();
				auto* listener = e2.AddComponent<ecs::components::AudioListenerComponent>();
				auto* camera_pos = e2.AddComponent<ecs::components::TransformComponent>();

				engine.GetAudioManager().CreateListener(e2.GetID());

				while (engine.GetRenderer().WindowIsOpen()) {

					engine.BeginFrame();

					Update();

					listener->forward	= camera.GetForward();
					listener->up		= camera.GetUp();

					transform->rotation.x += 0.2;
					transform->rotation.y += 0.2;

					engine.GetTransformSystem().SetRotation(entity, transform->rotation);
					camera_pos->position = camera.GetPosition();
					shaders.basic_shader.Bind();
					
					glUniformMatrix4fv(shaders.basic_shader.uniforms.uModel, 1, GL_FALSE, glm::value_ptr(transform->model));

					mm.DrawMesh<render::StaticMesh>(mesh->handle);
					
					engine.EndFrame();

				}
			}

		private:

			core::Engine& engine;
			EditorCamera camera{ to_f(engine.GetRenderer().GetAspectRatio()) };

		};
	}
}