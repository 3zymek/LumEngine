#pragma once
#include "core/core_pch.hpp"
namespace lum {
	namespace cmd {
		#define LumCmdTag \
			constexpr static LUMbool __lumcmd__ = true;

		template<typename T>
		concept Cmd = requires { T::__lumcmd__ == true; }&&
			std::is_trivially_copyable_v<T>&&
			std::is_trivially_destructible_v<T>;

		template <Cmd T, LUMsize MAX_SIZE>
		class CommandBuffer {
		public:

			T& operator[](LUMsize index) {
				return cmdbuffer[index];
			}

			inline void Push(const T& _cmd) noexcept {

				if (count >= MAX_SIZE) return;

				cmdbuffer[count] = _cmd;
				++count;

			}

			inline T* Begin() { return cmdbuffer; }

			inline LUMsize Count() const noexcept { return count; }

			inline void Clear() noexcept {

				std::memset(cmdbuffer, 0, sizeof(T) * MAX_SIZE);
				count = 0;

			}


		private:

			T cmdbuffer[MAX_SIZE]{};
			LUMsize count = 0;

		};
	}
}