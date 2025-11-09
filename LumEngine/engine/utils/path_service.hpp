#pragma once
#include <filesystem>
#include <string>
struct PathService {
	PathService() = delete;
	inline static void SetRoot(const std::string& root) noexcept {
		auto exe_path = std::filesystem::current_path();
		g_root = exe_path.parent_path();
		g_root = g_root / "lumengine" / root;
	}
	inline static std::string Resolve(const std::string& relative) noexcept {
		return (g_root / relative).string();
	}
private:
	inline static std::filesystem::path g_root;
};