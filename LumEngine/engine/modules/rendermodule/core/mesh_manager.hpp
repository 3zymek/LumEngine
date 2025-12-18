#pragma once
#include "rendermodule/details/render_define.hpp"
#include "glad/glad.h"
#include "glfw3.h"
#include "core/utils/handle_pool.hpp"
#include <vector>
#include <string>
#include <span>

namespace render {

	struct DynamicMesh {

		MeshHandle handle;

		GLuint VBO = 0;
		GLuint VAO = 0;
		GLuint EBO = 0;

		GLuint vertices_amount	= 0;
		GLuint indices_amount	= 0;

		GLuint max_vertices = 0;
		GLuint max_indices	= 0;

		std::vector<Vertex> vertices;
		std::vector<Indices> indices;

	};
	struct StaticMesh {

		MeshHandle handle;

		GLuint VBO = 0;
		GLuint VAO = 0;
		GLuint EBO = 0;

		GLuint vertices_amount	= 0;
		GLuint indices_amount	= 0;

		bool linked = false;

	};

	class MeshManager {

		using string_view	= std::string_view;
		template<typename T>
		using span = std::span<T>;

	public:

		[[nodiscard]] MeshHandle CreateStaticMesh	( span<Vertex> vert, span<Indices> indic );
		[[nodiscard]] MeshHandle CreateDynamicMesh	( size_t max_vert, size_t max_indic );


		template< MeshType >
		[[nodiscard]] MeshHandle CreateMeshFromFile	( string_view path );

		void SetDynamicMeshVertices	( MeshHandle, span<Vertex> );
		void SetDynamicMeshIndices	( MeshHandle, span<Indices> );
		
		template< MeshType >
		void DrawMesh(MeshHandle);

		template< MeshType >
		void DeleteMesh(MeshHandle);

	private:

		void UpdateDynamicData( DynamicMesh* );

		template< typename Mesh >
		void PreAllocGPU( Mesh&& );

		HandlePool< StaticMesh, MeshHandle > m_static_handles	{ MAX_MESHES_AMOUNT };
		HandlePool< DynamicMesh, MeshHandle > m_dynamic_handles	{ MAX_MESHES_AMOUNT };


	};

}
#include "rendermodule/core/mesh_manager.ipp"