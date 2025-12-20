#include <rendermodule/core/mesh_manager.hpp>
#include <rendermodule/details/render_define.hpp>
template<typename T>
	requires render::detail::MeshT<T>
inline void render::MeshManager::DrawMeshImpl(cstd::handle_pool<T, MeshHandle>& handles, MeshHandle handle) {
	if (!handles.Exists(handle))
		return;

	auto* mesh = handles.Get(handle);
	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, nullptr);

}
