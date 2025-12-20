#pragma once
#include <rendermodule/details/render_define.hpp>
#include <core/utils/handle_pool.hpp>
#include <vector>
#include <span>
#include <type_traits>
namespace render {
	enum class MeshType {
		Dynamic,
		Static
	};
	struct MeshHandle {

		using MeshID		= detail::MeshID;
		using Generation	= detail::Generation;

	public:

		MeshID		id = std::numeric_limits<MeshID>::max();
		Generation	generation = 0;
		MeshType	type;

	};
	struct StaticMesh {
	private:

		using VertexCount = detail::VertexCount;

	public:

		GLuint VAO = 0;
		GLuint VBO = 0;
		GLuint EBO = 0;

		Index		indices_amount = 0;
		VertexCount	vertices_amount = 0;

	};
	struct DynamicMesh {
	private:

		using VertexCount	= detail::VertexCount;
		using VertexArray	= detail::VertexArray;
		using IndexArray	= detail::IndexArray;

	public:

		GLuint VAO = 0;
		GLuint VBO = 0;
		GLuint EBO = 0;

		Index		indices_amount = 0;
		VertexCount	vertices_amount = 0;

		Index		max_indices = 0;
		VertexCount	max_vertices = 0;

		VertexArray	vertices;
		IndexArray	indices;

	};
	class MeshManager {

		using string_view = std::string_view;
		template<typename T>
		using span = std::span<T>;

	public:
		
		[[nodiscard]] MeshHandle CreateStaticMesh(std::span<Vertex>, std::span<Index>);
		[[nodiscard]] MeshHandle CreateDynamicMesh();

		template <MeshType>
		[[nodiscard]] MeshHandle LoadMeshFromFile(string_view path);

		template<MeshType>
		void DrawMesh(MeshHandle);

		template<MeshType>
		void DeleteMesh(MeshHandle);

	private:

		template<typename T>
		requires detail::MeshT<T>
		void DrawMeshImpl(cstd::handle_pool<T, MeshHandle>&, MeshHandle);

		void InitStaticMesh(StaticMesh&, span<Vertex>, span<Index>);
		void InitDynamicMesh(DynamicMesh&);
	};
	#include <rendermodule/core/mesh_manager.ipp>
}