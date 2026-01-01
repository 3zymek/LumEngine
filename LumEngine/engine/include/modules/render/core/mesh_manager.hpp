#pragma once
#include "render/render_common.hpp"
#include "core/utils/handle_pool.hpp"
#include "core/core_pch.hpp"
#include "render/essentials/mesh.hpp"
#include "glad/glad.h"
#include "glfw3.h"
namespace lum {
	namespace render {
		enum class MeshType {
			Dynamic,
			Static
		};
		struct MeshHandle {

			using MeshID		= lum::core::render::detail::MeshID;
			using Generation	= lum::core::render::detail::Generation;

		public:

			MeshID		id = std::numeric_limits<MeshID>::max();
			Generation	generation = 0;
			MeshType	type;

		};
		class MeshManager {

			using Vertex = lum::core::render::Vertex;
			using Index = lum::core::render::Index;
			using string_view = std::string_view;
			template<typename T>
			using span = std::span<T>;

		public:

			[[nodiscard]] MeshHandle CreateStaticMesh(span<Vertex>, span<Index>);
			[[nodiscard]] MeshHandle CreateDynamicMesh(size_t, size_t);

			void SetDynamicMeshVertices(MeshHandle, span<Vertex>);
			void SetDynamicMeshIndices(MeshHandle, span<Index>);

			template<typename T>
				requires lum::core::render::detail::MeshT<T>
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

			cstd::handle_pool<StaticMesh, MeshHandle>	m_static_handles{ lum::core::render::MAX_MESHES_AMOUNT};
			cstd::handle_pool<DynamicMesh, MeshHandle>	m_dynamic_handles{ lum::core::render::MAX_MESHES_AMOUNT };

		};
		#include "render/core/mesh_manager.ipp"
	}
}