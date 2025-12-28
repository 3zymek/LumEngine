#pragma once
#include "render/render_common.hpp"
#include "core/utils/handle_pool.hpp"
#include "core/core_pch.hpp"
#include "render/essentials/mesh.hpp"
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
	class MeshManager {

		using string_view = std::string_view;
		template<typename T>
		using span = std::span<T>;

	public:
		
		[[nodiscard]] MeshHandle CreateStaticMesh(std::span<Vertex>, std::span<Index>);
		[[nodiscard]] MeshHandle CreateDynamicMesh(size_t, size_t);

		void SetDynamicMeshVertices(MeshHandle, std::span<Vertex>);
		void SetDynamicMeshIndices(MeshHandle, std::span<Index>);

		template<typename T>
			requires detail::MeshT<T>
		[[nodiscard]] MeshHandle LoadMeshFromFile(string_view path);

		template<typename T>
		void DrawMesh(MeshHandle);

		template<typename T>
		void DeleteMesh(MeshHandle);

	private:

		template<typename T>
		void DrawMeshImpl(cstd::handle_pool<T, MeshHandle>&, MeshHandle);

		void InitStaticMesh(StaticMesh&, span<Vertex>, span<Index>);
		void InitDynamicMesh(DynamicMesh&);

		cstd::handle_pool<StaticMesh, MeshHandle>	m_static_handles{ MAX_MESHES_AMOUNT };
		cstd::handle_pool<DynamicMesh, MeshHandle>	m_dynamic_handles{ MAX_MESHES_AMOUNT };
		
	};
	#include "render/core/mesh_manager.ipp"
}