#pragma once
#include <type_traits>

template<typename T>
concept System = !std::is_arithmetic_v<T>;

template<System T>
class BaseSystem {
	inline void Update() {
		static_cast<T*>(this)->UpdateImplementation();
	}
};