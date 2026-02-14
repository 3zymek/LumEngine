#pragma once
#include "core/core_pch.hpp"
#include "stb_image.h"
#include "core/utils/lum_assert.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace lum {

	namespace detail {

		namespace fs = std::filesystem;
		
		inline fs::path gAssetsRoot = fs::current_path().parent_path() / "LumEngine" / "assets";

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

		static TextureData LoadTexture(ccharptr filename, bool& success) {

			String absolute_path = (texturePath / String(filename)).lexically_normal().string();

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
		
		static void LoadMesh(ccharptr filename, bool& success) {
			
			// TODO Implement this function

		}

		static String LoadAudio(StringView filename) {

			detail::fs::path file = audioPath / filename;
			if (!detail::fs::exists(file)) {
				LUM_LOG_ERROR("Couldn't localize audio file named {}");
				return "";
			}

			return file.lexically_normal().string();
		}

		static String LoadInternalShader(StringView filename, bool& success) {

			String file = (internalShaderPath / filename).lexically_normal().string();

			if (!detail::fs::exists(file)) {
				success = false;
				return "";
			}

			std::ifstream loaded_file(file);
			std::ifstream defines(shaderDefine);
			if (!loaded_file.is_open() || !defines.is_open()) {
				success = false;
				return "";
			}

			String version;
			std::getline(loaded_file, version);

			std::stringstream ss;
			ss << version << '\n';
			ss << defines.rdbuf() << '\n';
			ss << loaded_file.rdbuf();

			success = true;
			return ss.str();
		}

		static String LoadExternalShader(StringView filename) {

			String file = (externalShadersPath / filename).lexically_normal().string();

			if (!detail::fs::exists(file)) {
				LUM_LOG_ERROR("Couldn't localize shader file named {}");
				return "";
			}

			std::ifstream loaded_file(file);
			std::ifstream defines(shaderDefine);
			if (!loaded_file.is_open() || !defines.is_open()) {
				LUM_LOG_ERROR("Couldn't open shader file named {}");
				return "";
			}

			String version;
			std::getline(loaded_file, version);

			std::stringstream ss;
			ss << version << '\n';
			ss << defines.rdbuf() << '\n';
			ss << loaded_file.rdbuf();

			return ss.str();
		}

	private:

		static inline detail::fs::path	internalShaderPath	= detail::fs::current_path().parent_path() / "LumEngine" / "engine" / "include" / "modules" / "rhi" / "shaders";
		static inline String			shaderDefine		= (detail::fs::current_path().parent_path() / "LumEngine" / "engine" / "include" / "core" / "shaders_define.h").lexically_normal().string();
		static inline detail::fs::path	externalShadersPath	= detail::gAssetsRoot	/ "shader";
		static inline detail::fs::path	audioPath			= detail::gAssetsRoot	/ "audio";
		static inline detail::fs::path	texturePath			= detail::gAssetsRoot	/ "texture";
		static inline detail::fs::path	modelPath			= detail::gAssetsRoot / "models";

		AssetService(const AssetService&)	= delete;
		AssetService(AssetService&&)		= delete;
		AssetService() = default;

	};
}