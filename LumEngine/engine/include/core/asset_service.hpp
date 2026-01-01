#pragma once
#include "core/core_pch.hpp"
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
	
	class AssetService {
	public:

		static std::string LoadShader(std::string_view file_name) {
			auto file = shader_path / file_name;
			if (!detail::fs::exists(file))
				throw std::runtime_error(std::string("Couldn't localize shader file named ") + file_name.data());


		}

		static void LoadTexture(std::string_view file_name) {

		}
		
		static std::string LoadAudio(std::string_view file_name) {
			auto file = audio_path / file_name;
			if (!detail::fs::exists(file))
				throw std::runtime_error(std::string("Couldn't localize audio file named ") + file_name.data());

			return file.lexically_normal().string();
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