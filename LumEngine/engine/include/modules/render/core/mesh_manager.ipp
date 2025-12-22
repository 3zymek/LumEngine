#include <include/modules/render/core/mesh_manager.hpp>
#include <include/modules/render/render_common.hpp>

template<typename T>
void render::MeshManager::DrawMesh(MeshHandle handle) {
	if constexpr (std::is_same_v<T, StaticMesh>) {
		DrawMeshImpl<T>(m_static_handles, handle);
	}
	else if constexpr (std::is_same_v<T, DynamicMesh>) {
		DrawMeshImpl<T>(m_dynamic_handles, handle);
	}
	else static_assert(false, "unsupported mesh type in DrawMesh");

}
template<typename T>
void render::MeshManager::DeleteMesh(MeshHandle handle) {
	if constexpr (std::is_same_v<T, StaticMesh>) {
		if (!m_static_handles.Exists(handle))
			return;
		m_static_handles.DeleteHandle(handle);
	}
	else if constexpr (std::is_same_v<T, DynamicMesh>) {
		if (!m_dynamic_handles.Exists(handle))
			return;
		m_dynamic_handles.DeleteHandle(handle);
	}
	else static_assert(detail::MeshT<T>, "unsupported mesh type in DeleteMesh");
}



template<typename T>
inline void render::MeshManager::DrawMeshImpl(cstd::handle_pool<T, MeshHandle>& handles, MeshHandle handle) {
	if (!handles.Exists(handle))
		return;

	auto* mesh = handles.Get(handle);
	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, mesh->indices_amount, GL_UNSIGNED_INT, nullptr);

}
