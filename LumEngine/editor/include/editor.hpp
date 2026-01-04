#pragma once
#include "core/engine.hpp"
#include "editor_camera.hpp"
namespace lum {
	namespace editor {

		class Editor { 
		public:

			Editor(core::Engine& eng) : engine(eng) {}

			void Update() {

				camera.Update();
				
			}

			void Run() {

				auto& shaders = engine.GetShaderManager();
				render::ShaderHandle basic_shader = shaders.CreateShader("basic.vert", "basic.frag");
				auto& mm = engine.GetMeshManager();
				std::vector<render::Vertex> vertices = {
					{ {1, 1, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0} },
					{ {-1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {0, 0} },
					{ {0, -1, 0}, {0, 0, 1}, {0, 0, 0}, {0, 0} }
				};
				std::vector<render::Index> indices = { 0, 1, 2 };
				render::MeshHandle mesh_handle = mm.CreateStaticMesh(vertices, indices);

				glm::vec3 model_pos = { 0, 0, 0 };
				glm::vec3 model_scale = { 1, 1, 1 };

				while (engine.GetRenderer().WindowIsOpen()) {

					engine.BeginFrame();

					Update();

					shaders.UseShader(basic_shader);

					glm::mat4 model = glm::mat4(1);
					model = glm::translate(model, model_pos);
					model = glm::scale(model, model_scale);
					GLuint modelPos = glGetUniformLocation(shaders.GetProgram(basic_shader), "uModel");
					glUniformMatrix4fv(modelPos, 1, GL_FALSE, glm::value_ptr(model));

					mm.DrawMesh<render::StaticMesh>(mesh_handle);
					
					engine.EndFrame();

				}
			}

		private:

			core::Engine& engine;
			EditorCamera camera{ (float)engine.GetRenderer().GetAspectRatio() };

		};
	}
}