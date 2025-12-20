#include <rendermodule/core/mesh_manager.hpp>
namespace render {
	MeshHandle MeshManager::CreateStaticMesh(std::span<Vertex> vertices, std::span<Index> indices) {
		StaticMesh mesh;

		InitStaticMesh(mesh, vertices, indices);

		MeshHandle h;
		return h;

	}

	void MeshManager::InitStaticMesh(StaticMesh& mesh, span<Vertex> vertices, span<Index> indices) {

		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.VBO);
		glGenBuffers(1, &mesh.EBO);

		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices_amount * sizeof(Vertex), nullptr, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices_amount * sizeof(Index), nullptr, GL_STATIC_DRAW);

	}

}