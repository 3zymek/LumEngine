#pragma once
#include "core/core_pch.hpp"
#include "glad/glad.h"
namespace lum {
	namespace render {

		class UniformBuffer {
		public:

			UniformBuffer(GLuint slot, GLsizei size) { Init(slot, size); }
			~UniformBuffer() { if (UBO) glDeleteBuffers(1, &UBO); }

			void Update(const void* data, GLsizeiptr size, GLintptr offset) {

				glBindBuffer(GL_UNIFORM_BUFFER, UBO);
				glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);

			}

		private:

			void Init(GLuint slot, GLsizei size) {

				glGenBuffers(1, &UBO);
				glBindBuffer(GL_UNIFORM_BUFFER, UBO);
				glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_UNIFORM_BUFFER, slot, UBO);

			}

			GLuint UBO = 0;

		};

	}

}
