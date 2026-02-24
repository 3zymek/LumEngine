#pragma once

#include "core/core_common.hpp"

namespace lum {

	namespace fmt {
		
		enum class ETokenType : byte {
			
			Identifier,
			LBracket,
			RBracket,
			Parameter,
			Component,
			Colon,
			Dollar,
			String,
			Number,
			EndOfLine
			
		};
		
		struct FToken {

			ETokenType mType;
			String mValue;

		};

		inline bool IsValidFormat( StringView str ) {
		
			usize dot = str.find_last_of('.');

			if (dot == StringView::npos)
				return false;

			StringView result = str.substr(dot + 1);
			if (result == "lsc" || result == "lmt")
				return true;

			return false;
		}
		
	} // namespace lum::fmt

} // namespace lum