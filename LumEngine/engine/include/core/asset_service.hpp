#pragma once
#include "core/core_pch.hpp"
#include "stb_image.h"
namespace lum {

	namespace detail {

		namespace fs = std::filesystem;
		
		inline fs::path g_assets_root = fs::current_path().parent_path() / "lumengine" / "assets";

	}

	enum class AssetType {
		Shader,
		Texture,
		Audio
	};

	struct TextureData {
		int width = 0;
		int height = 0;
		int color_channels = 0;
		std::vector<uint8_t> pixels;
	};
	
	class AssetService {
	public:

		static TextureData LoadTexture(const char* file_name) {

			auto path = texture_path / std::string(file_name);
			std::string absolute_path = path.lexically_normal().string();

			TextureData tx;
			int ignore;
			unsigned char* data = stbi_load(absolute_path.c_str(), &tx.width, &tx.height, &ignore, STBI_rgb_alpha);
			assert(tx.width > 0 & tx.height > 0);
			tx.color_channels = STBI_rgb_alpha;

			if (!data) {
				LOG_ERROR(std::format("Failed to load texture {}", file_name));
				return TextureData{};
			}

			size_t size = tx.width * tx.height * tx.color_channels;
			tx.pixels.resize(size);
			std::memcpy(tx.pixels.data(), data, size);

			stbi_image_free(data);
			return tx;
		}
		
		inline static std::string LoadAudio(std::string_view file_name) {
			auto file = audio_path / file_name;
			if (!detail::fs::exists(file)) {
				LOG_ERROR(std::format("Couldn't localize audio file named {}", file_name.data()));
				return "";
			}

			return file.lexically_normal().string();
		}

		static inline std::string LoadShader(std::string_view file_name) {
			auto file = (shader_path / file_name).lexically_normal().string();
			if (!detail::fs::exists(file)) {
				LOG_ERROR(std::format("Couldn't localize shader file named {}", file_name.data()));
				return "";
			}

			std::ifstream loaded_file(file);
			if (!loaded_file.is_open()) {
				LOG_ERROR(std::format("Couldn't open shader file named {}", file_name.data()));
				return "";
			}

			std::stringstream ss;
			ss << loaded_file.rdbuf();
			
			return ss.str();
		}

	private:

		static inline detail::fs::path shader_path	= detail::g_assets_root	/ "shader";
		static inline detail::fs::path audio_path	= detail::g_assets_root	/ "audio";
		static inline detail::fs::path texture_path	= detail::g_assets_root	/ "texture";

		AssetService(const AssetService&)	= delete;
		AssetService(AssetService&&)		= delete;
		AssetService() = default;

	};
}