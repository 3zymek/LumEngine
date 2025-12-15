#include "rendermodule/core/mesh_manager.hpp"
#include "core_defines.hpp"

namespace render {

	MeshHandle MeshManager::CreateStaticMesh(size_t vertices_count, size_t indices_count) {
		
		MeshHandle new_handle = GenerateID<MeshHandle, 0>::Get();
		StaticMesh mesh;



		return new_handle;


	}


	void MeshManager::PreAllocGPU(Mesh& mesh, size_t vertices_count, size_t indices_count) {

		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.VBO);
		glGenBuffers(1, &mesh.EBO);

		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(Vertex), nullptr, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(Indices), nullptr, GL_STATIC_DRAW);



	}

}

