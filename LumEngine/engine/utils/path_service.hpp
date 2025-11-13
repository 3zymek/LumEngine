#pragma once
#include <filesystem>
#include <string>
struct PathService {
	PathService() = delete;
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
	inline static std::string Resolve(const std::string& relative) noexcept {
		return (g_root / relative).lexically_normal().string();
	}
private:
	inline static std::filesystem::path g_root;
};