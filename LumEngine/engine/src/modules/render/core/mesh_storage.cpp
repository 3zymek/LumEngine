#include "render/core/mesh_storage.hpp"
#include "render/essentials/mesh.hpp"
namespace lum {
	namespace render {
		MeshHandle MeshStorage::CreateStaticMesh( std::span<Vertex> vertices, std::span<Index> indices ) {
			StaticMesh mesh;

			mesh.vertices_amount = vertices.size();
			mesh.indices_amount = indices.size();
			InitStaticMesh(mesh, vertices, indices);

			return m_static_handles.CreateHandle(std::move(mesh));

		}
		MeshHandle MeshStorage::CreateDynamicMesh( size_t max_vertices, size_t max_indices ) {
			DynamicMesh mesh;

			mesh.max_vertices = max_vertices;
			mesh.max_indices = max_indices;
			InitDynamicMesh(mesh);

			return m_dynamic_handles.CreateHandle(std::move(mesh));

		}

		void MeshStorage::SetDynamicMeshVertices( MeshHandle handle, std::span<Vertex> vertices ) {
			if (!m_dynamic_handles.Exists(handle)) {
				LOG_ERROR("Dynamic handle does not exists");
				return;
			}

			auto* mesh = m_dynamic_handles.Get(handle);

			if (vertices.size() < mesh->max_vertices)
				return;

			mesh->vertices.assign(vertices.begin(), vertices.end());
			mesh->vertices_amount = mesh->vertices.size();

		} //
		void MeshStorage::SetDynamicMeshIndices( MeshHandle handle, std::span<Index> indices ) {
			if (!m_dynamic_handles.Exists(handle)) {
				LOG_ERROR("Dynamic handle does not exists");
				return;
			}

			auto* mesh = m_dynamic_handles.Get(handle);

			if (mesh->indices_amount < indices.size() || !mesh) {
				LOG_ERROR("Too much indices");
				return;
			}

			mesh->indices.resize(indices.size());
			std::memcpy(mesh->indices.data(), indices.data(), sizeof(uint32_t) * indices.size());
			mesh->indices_amount = mesh->indices.size();

		}

		void MeshStorage::InitStaticMesh	( StaticMesh& mesh, span<Vertex> vertices, span<Index> indices ) {

			glGenVertexArrays(1, &mesh.VAO);
			glGenBuffers(1, &mesh.VBO);
			glGenBuffers(1, &mesh.EBO);

			glBindVertexArray(mesh.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
			glBufferData(GL_ARRAY_BUFFER, mesh.vertices_amount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);


			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices_amount * sizeof(Index), indices.data(), GL_STATIC_DRAW);

		}
		void MeshStorage::InitDynamicMesh	( DynamicMesh& mesh ) {

			glGenVertexArrays(1, &mesh.VAO);
			glGenBuffers(1, &mesh.VBO);
			glGenBuffers(1, &mesh.EBO);

			glBindVertexArray(mesh.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
			glBufferData(GL_ARRAY_BUFFER, mesh.max_vertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.max_indices * sizeof(Index), nullptr, GL_DYNAMIC_DRAW);

		}
	}
}