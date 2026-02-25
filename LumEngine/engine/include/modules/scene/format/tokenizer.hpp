#pragma once

#include "scene/format/common.hpp"

namespace lum::fmt {

	class Tokenizer {
	public:
		
		Tokenizer( ) { }

		void Tokenize(StringView str);
		std::vector<FToken>& GetTokens() noexcept { return mTokens; }

	private:

		std::vector<FToken> mTokens;

	};

} // namespace lum::fmt