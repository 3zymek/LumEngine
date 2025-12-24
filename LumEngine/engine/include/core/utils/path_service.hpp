#pragma once
#include "core/core_pch.hpp"
namespace cstd {
	struct PathService {
		inline static void SetRoot(std::string_view root) noexcept {
			namespace fs = std::filesystem;
			auto path = fs::current_path();

			while (!path.empty()) {

				if (fs::exists(path / "lumengine" / root)) {
					g_root = path / "lumengine" / root;
					return;
				}
				path = path.parent_path();

			}
		}
		inline static std::string GetPath(std::string_view relative) noexcept {
			return (g_root / relative.data()).lexically_normal().string();
		}
	private:
		PathService() { SetRoot("assets"); std::cout << g_root; }
		inline static std::filesystem::path g_root;
	};
}