#pragma once
#include "rendermodule/details/render_define.hpp"
#include "glad/glad.h"
#include "glfw3.h"
#include <vector>
#include <string>
namespace render {

	struct DynamicMesh {

		MeshHandle handle = 0;

		GLuint VBO = 0;
		GLuint VAO = 0;
		GLuint EBO = 0;

		std::vector<Vertex> vertices;
		std::vector<detail::Indices> indices;
	};
	struct StaticMesh {

	};

	class MeshManager {

		using string_view = std::string_view;
		using Indices = detail::Indices;

	public:

		[[nodiscard]] MeshHandle CreateStaticMesh	( size_t, size_t );
		[[nodiscard]] MeshHandle CreateDynamicMesh	( size_t, size_t );
		[[nodiscard]] MeshHandle CreateMeshFromFile	( string_view );
		void LinkMesh	( MeshHandle );
		void DeleteMesh	( MeshHandle );

		void SetMeshVertices( MeshHandle, std::vector<Vertex>& );
		void SetMeshIndices	( MeshHandle, std::vector<Indices>& );

	private:

		void PreAllocGPU(Mesh&, size_t, size_t);

	};

}