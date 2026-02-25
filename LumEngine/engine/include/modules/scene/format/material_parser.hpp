#pragma once

#include "core/core_common.hpp"

namespace lum::fmt {

	class Tokenizer;

	struct MaterialData {

		std::optional<String> mAlbedoTex;
		std::optional<String> mNormalTex;
		std::optional<String> mRoughnessTex;
		std::optional<String> mMetallicTex;

		std::optional<glm::vec3> mBaseColor;
		std::optional<float32> mRoughnessValue = 0.0f;
		std::optional<float32> mMetallicValue = 0.0f;

	};

	class MaterialParser {
	public:

		MaterialParser( Tokenizer& tokenizer ) : mTokenizer( tokenizer ) { }
		
		void Parse( MaterialData& material );

	private:

		Tokenizer& mTokenizer;

	};



}