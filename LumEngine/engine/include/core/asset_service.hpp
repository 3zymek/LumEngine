#pragma once
#include "core/core_pch.hpp"
#include "stb_image.h"
#include "core/lum_assert.hpp"
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
		int colorChannels = 0;
		std::vector<uint8_t> pixels;
	};
	
	class AssetService {
	public:

		static TextureData load_texture(ccharptr file_name) {

			auto path = texture_path / std::string(file_name);
			std::string absolute_path = path.lexically_normal().string();

			TextureData tx;
			int ignore;
			unsigned char* data = stbi_load(absolute_path.c_str(), &tx.width, &tx.height, &ignore, 4);
			assert(tx.width > 0 && tx.height > 0);
			tx.colorChannels = 4;

			if (!data) {
				LUM_LOG_ERROR(std::format("Failed to load texture {}", file_name));
				return TextureData{};
			}

			size_t size = tx.width * tx.height * tx.colorChannels;
			tx.pixels.resize(size);
			std::memcpy(tx.pixels.data(), data, size);

			stbi_image_free(data);
			return tx;
		}
		
		inline static std::string load_audio(std::string_view file_name) {

			auto file = audio_path / file_name;
			if (!detail::fs::exists(file)) {
				LUM_LOG_ERROR(std::format("Couldn't localize audio file named {}", file_name.data()));
				return "";
			}

			return file.lexically_normal().string();
		}

		static std::string LoadInternalShader(std::string_view file_name) {

			auto file = (internal_shader_path / file_name).lexically_normal().string();

			if (!detail::fs::exists(file)) {
				LUM_LOG_ERROR(std::format("Couldn't localize shader file named {}", file_name.data()));
				return "";
			}

			std::ifstream loaded_file(file);
			std::ifstream defines(shader_define);
			if (!loaded_file.is_open() || !defines.is_open()) {
				LUM_LOG_ERROR(std::format("Couldn't open shader file named {}", file_name.data()));
				return "";
			}

			std::string version;
			std::getline(loaded_file, version);

			std::stringstream ss;
			ss << version << '\n';
			ss << defines.rdbuf() << '\n';
			ss << loaded_file.rdbuf();

			return ss.str();
		}

		static std::string LoadExternalShader(std::string_view file_name) {

			auto file = (external_shader_path / file_name).lexically_normal().string();

			if (!detail::fs::exists(file)) {
				LUM_LOG_ERROR(std::format("Couldn't localize shader file named {}", file_name.data()));
				return "";
			}

			std::ifstream loaded_file(file);
			std::ifstream defines(shader_define);
			if (!loaded_file.is_open() || !defines.is_open()) {
				LUM_LOG_ERROR(std::format("Couldn't open shader file named {}", file_name.data()));
				return "";
			}

			std::string version;
			std::getline(loaded_file, version);

			std::stringstream ss;
			ss << version << '\n';
			ss << defines.rdbuf() << '\n';
			ss << loaded_file.rdbuf();

			return ss.str();
		}

	private:

		static inline detail::fs::path internal_shader_path = detail::fs::current_path().parent_path() / "lumengine" / "engine" / "include" / "modules" / "rhi" / "shaders";
		static inline std::string shader_define = (detail::fs::current_path().parent_path() / "lumengine" / "engine" / "include" / "core" / "shaders_define.h").lexically_normal().string();
		static inline detail::fs::path external_shader_path	= detail::g_assets_root	/ "shader";
		static inline detail::fs::path audio_path	= detail::g_assets_root	/ "audio";
		static inline detail::fs::path texture_path	= detail::g_assets_root	/ "texture";

		AssetService(const AssetService&)	= delete;
		AssetService(AssetService&&)		= delete;
		AssetService() = default;

	};
}