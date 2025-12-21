#include "rendermodule/core/mesh_manager.hpp"
#include "rendermodule/essentials/mesh.hpp"
namespace render {
	MeshHandle MeshManager::CreateStaticMesh(std::span<Vertex> vertices, std::span<Index> indices) {
		StaticMesh mesh;

		mesh.vertices_amount = vertices.size();
		mesh.indices_amount = indices.size();
		InitStaticMesh(mesh, vertices, indices);

		return m_static_handles.CreateHandle(std::move(mesh));

	}
	MeshHandle MeshManager::CreateDynamicMesh(size_t max_vertices, size_t max_indices) {
		DynamicMesh mesh;
		
		mesh.max_vertices = max_vertices;
		mesh.max_indices = max_indices;
		InitDynamicMesh(mesh);

		return m_dynamic_handles.CreateHandle(std::move(mesh));

	}

	void MeshManager::SetDynamicMeshVertices(MeshHandle handle, std::span<Vertex> vertices) {
		if (!m_dynamic_handles.Exists(handle)) {
			LOG_ERROR("[SET] dynamic handle does not exists");
			return;
		}

		auto* mesh = m_dynamic_handles.Get(handle);

		if (vertices.size() < mesh->max_vertices)
			return;
		
		mesh->vertices.assign(vertices.begin(), vertices.end());
		mesh->vertices_amount = mesh->vertices.size();
		
	}
	void MeshManager::SetDynamicMeshIndices(MeshHandle handle, std::span<Index> indices) {
		if (!m_dynamic_handles.Exists(handle)) {
			LOG_ERROR("[SET] dynamic handle does not exists at SetDynamicMeshIndices()");
			return;
		}
		
		auto* mesh = m_dynamic_handles.Get(handle);

		if (mesh->indices_amount < indices.size()) {
			LOG_ERROR("[SET] too much indices at SetDynamicMeshIndices()");
			return;
		}

		mesh->indices.assign(indices.begin(), indices.end());
		mesh->indices_amount = mesh->indices.size();

	}

	void MeshManager::InitStaticMesh(StaticMesh& mesh, span<Vertex> vertices, span<Index> indices) {

		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.VBO);
		glGenBuffers(1, &mesh.EBO);

		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices_amount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices_amount * sizeof(Index), indices.data(), GL_STATIC_DRAW);

	}
	void MeshManager::InitDynamicMesh(DynamicMesh& mesh) {

		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.VBO);
		glGenBuffers(1, &mesh.EBO);

		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.max_vertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.max_indices * sizeof(Index), nullptr, GL_DYNAMIC_DRAW);

	}

}