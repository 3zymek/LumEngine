#pragma once
#include "render/render_common.hpp"
namespace lum {
	namespace render {
		struct StaticMesh {
		private:

			using VertexCount	= lum::core::render::detail::VertexCount;
			using Index			= lum::core::render::Index;

		public:

			GLuint VAO = 0;
			GLuint VBO = 0;
			GLuint EBO = 0;

			Index		indices_amount = 0;
			VertexCount	vertices_amount = 0;

		};
		struct DynamicMesh {
		private:

			using VertexCount	= lum::core::render::detail::VertexCount;
			using VertexArray	= lum::core::render::detail::VertexArray;
			using IndexArray	= lum::core::render::detail::IndexArray;
			using VertexCount	= lum::core::render::detail::VertexCount;
			using Index			= lum::core::render::Index;

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

			bool updated = false;

		};
	}
}