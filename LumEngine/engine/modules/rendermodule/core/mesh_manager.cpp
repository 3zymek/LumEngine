#include "rendermodule/core/mesh_manager.hpp"
#include "core_defines.hpp"

namespace render {

	MeshHandle MeshManager::CreateDynamicMesh(size_t vertices_amount, size_t indices_amount) {
		DynamicMesh mesh;
		
		mesh.indices_amount = indices_amount;
		mesh.indices.reserve(indices_amount);
		mesh.vertices_amount = vertices_amount;
		mesh.vertices.reserve(vertices_amount);

		PreAllocGPU(mesh);
		
		return m_dynamic_handles.CreateHandle(mesh);

	}
	void MeshManager::SetDynamicMeshVertices(MeshHandle& handle, span<Vertex>& vertices) {
		if (!m_dynamic_handles.Exists(handle))
			return;

		auto* mesh = m_dynamic_handles.Get(handle);
		mesh->vertices.assign(vertices.begin(), vertices.end());
		mesh->vertices_amount = vertices.size();

		UpdateDynamicData(mesh);

	}
	void MeshManager::SetDynamicMeshIndices(MeshHandle& handle, span<Indices>& indices) {
		if (!m_dynamic_handles.Exists(handle))
			return;

		auto* mesh = m_dynamic_handles.Get(handle);
		mesh->indices.assign(indices.begin(), indices.end());
		mesh->indices_amount = indices.size();

		UpdateDynamicData(mesh);

	}

	void MeshManager::UpdateDynamicData(DynamicMesh* mesh) {
		
		glBindVertexArray(mesh->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->vertices_amount * sizeof(Vertex), mesh->vertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->indices_amount * sizeof(Vertex), mesh->indices.data());

	}

}

