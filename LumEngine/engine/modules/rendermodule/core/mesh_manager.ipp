#include "rendermodule/core/mesh_manager.hpp"
#include "core_defines.hpp"

namespace render {

	MeshHandle MeshManager::CreateStaticMesh(span<Vertex> vertices, span<Indices> indices) {

		StaticMesh mesh;
		mesh.indices_amount = indices.size();
		mesh.vertices_amount = vertices.size();

		PreAllocGPU(mesh);

		return m_static_handles.CreateHandle(mesh);

	}
	template< >
	MeshHandle MeshManager::CreateMeshFromFile<MeshType::Static>( string_view path ) {
		////////////////////////////
	}

	template< >
	void MeshManager::DrawMesh<MeshType::Dynamic>(MeshHandle& handle) {
		if (!m_dynamic_handles.Exists(handle))
			return;

		auto* mesh = m_dynamic_handles.Get(handle);
		glBindVertexArray(mesh->VAO);
		glDrawElements(GL_TRIANGLES, mesh->indices_amount, GL_UNSIGNED_INT, nullptr);

	}
	template< >
	void MeshManager::DrawMesh<MeshType::Static>(MeshHandle& handle) {
		if (!m_static_handles.Exists(handle))
			return;

		auto* mesh = m_static_handles.Get(handle);
		glBindVertexArray(mesh->VAO);
		glDrawElements(GL_TRIANGLES, mesh->indices_amount, GL_UNSIGNED_INT, nullptr);

	}

	template< >
	void MeshManager::DeleteMesh<MeshType::Dynamic>(MeshHandle& handle) {
		if (!m_dynamic_handles.Exists(handle))
			return;

		m_dynamic_handles.DeleteHandle(handle);

	}
	template< >
	void MeshManager::DeleteMesh<MeshType::Static>(MeshHandle& handle) {
		if (!m_static_handles.Exists(handle))
			return;

		m_static_handles.DeleteHandle(handle);

	}


	template< typename Mesh >
	void MeshManager::PreAllocGPU(Mesh&& mesh) {

		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.VBO);
		glGenBuffers(1, &mesh.EBO);

		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices_amount * sizeof(Vertex), nullptr, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices_amount * sizeof(Indices), nullptr, GL_STATIC_DRAW);

	}

}

