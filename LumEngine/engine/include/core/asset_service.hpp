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
		int32 width = 0;
		int32 height = 0;
		int32 colorChannels = 0;
		std::vector<uint8> pixels;
	};
	
	class AssetService {
	public:

		static TextureData load_texture(ccharptr file_name, bool& success) {

			string absolute_path = (texture_path / string(file_name)).lexically_normal().string();

			TextureData tx;
			int32 ignore;
			ucharptr data = stbi_load(absolute_path.c_str(), &tx.width, &tx.height, &ignore, 4);
			tx.colorChannels = 4;

			if (!data) {
				success = false;
				return tx;
			}
			success = true;

			usize size = tx.width * tx.height * tx.colorChannels;
			tx.pixels.resize(size);

			LUM_ASSERT(size > 0 && data != nullptr, "Texture source data is null");

			std::memcpy(tx.pixels.data(), data, size);

			stbi_image_free(data);
			return tx;
		}
		
		inline static string load_audio(string_view file_name) {

			detail::fs::path file = audio_path / file_name;
			if (!detail::fs::exists(file)) {
				LUM_LOG_ERROR("Couldn't localize audio file named {}");
				return "";
			}

			return file.lexically_normal().string();
		}

		static string load_internal_shader(string_view file_name, bool& success) {

			string file = (internal_shader_path / file_name).lexically_normal().string();

			if (!detail::fs::exists(file)) {
				success = false;
				return "";
			}

			std::ifstream loaded_file(file);
			std::ifstream defines(shader_define);
			if (!loaded_file.is_open() || !defines.is_open()) {
				success = false;
				return "";
			}

			string version;
			std::getline(loaded_file, version);

			std::stringstream ss;
			ss << version << '\n';
			ss << defines.rdbuf() << '\n';
			ss << loaded_file.rdbuf();

			success = true;
			return ss.str();
		}

		static string load_external_shader(string_view file_name) {

			string file = (external_shader_path / file_name).lexically_normal().string();

			if (!detail::fs::exists(file)) {
				LUM_LOG_ERROR("Couldn't localize shader file named {}");
				return "";
			}

			std::ifstream loaded_file(file);
			std::ifstream defines(shader_define);
			if (!loaded_file.is_open() || !defines.is_open()) {
				LUM_LOG_ERROR("Couldn't open shader file named {}");
				return "";
			}

			string version;
			std::getline(loaded_file, version);

			std::stringstream ss;
			ss << version << '\n';
			ss << defines.rdbuf() << '\n';
			ss << loaded_file.rdbuf();

			return ss.str();
		}

	private:

		static inline detail::fs::path internal_shader_path = detail::fs::current_path().parent_path() / "lumengine" / "engine" / "include" / "modules" / "rhi" / "shaders";
		static inline string shader_define = (detail::fs::current_path().parent_path() / "lumengine" / "engine" / "include" / "core" / "shaders_define.h").lexically_normal().string();
		static inline detail::fs::path external_shader_path	= detail::g_assets_root	/ "shader";
		static inline detail::fs::path audio_path	= detail::g_assets_root	/ "audio";
		static inline detail::fs::path texture_path	= detail::g_assets_root	/ "texture";

		AssetService(const AssetService&)	= delete;
		AssetService(AssetService&&)		= delete;
		AssetService() = default;

	};
}