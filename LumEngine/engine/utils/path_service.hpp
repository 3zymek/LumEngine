#pragma once
#include <filesystem>
#include <string>
struct PathService {
	PathService() = delete;
	inline static void SetRoot(const std::string& root) noexcept {
		g_root = std::filesystem::absolute(root);
	}
	inline static std::string Resolve(const std::string& relative) noexcept {
		return (g_root / relative).string();
	}
private:
	inline static std::filesystem::path g_root;
};