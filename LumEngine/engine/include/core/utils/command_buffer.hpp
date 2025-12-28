#pragma once
#include "core/core_pch.hpp"
namespace cmd {

#define LumCmdTag \
	constexpr static bool thisIsCommand = true;

	template<typename T>
	concept Cmd = requires { T::thisIsCommand; }&&
		std::is_trivially_copyable_v<T> &&
		std::is_trivially_destructible_v<T>;

	template <Cmd T, size_t MAX_SIZE>
	class CommandBuffer {
	public:

		T& operator[](size_t index) {
			return cmdbuffer[index];
		}

		inline void Push(const T& _cmd) noexcept {
			
			if (count >= MAX_SIZE) return;

			cmdbuffer[count] = _cmd;
			++count;

		}

		inline T* Begin() { return cmdbuffer; }

		inline size_t Count() const noexcept { return count; }

		inline void Clear() noexcept {

			std::memset(cmdbuffer, 0, sizeof(T) * MAX_SIZE);
			count = 0;

		}


	private:

		T cmdbuffer[MAX_SIZE]{};
		size_t count = 0;

	};

}