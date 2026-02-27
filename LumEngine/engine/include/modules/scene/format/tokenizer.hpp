//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Tokenizer for .lsc and .lmt format files.
//
//=============================================================================//
#pragma once

#include "scene/format/common.hpp"

namespace lum::fmt {

    /* @brief Splits raw file content into a sequence of tokens for parsing.
    * Used as the first stage of the .lsc and .lmt file parsing pipeline.
    */
    class Tokenizer {
    public:

        Tokenizer( ) = default;

        /* @brief Tokenizes the given string into a flat token list.
         * @param str Raw file content to tokenize.
         */
        void Tokenize( StringView str) ;

        /* @brief Returns the token list produced by the last Tokenize call. */
        std::vector<FToken>& GetTokens( ) noexcept { return mTokens; }

    private:
    
        std::vector<FToken> mTokens;
    
    };

} // namespace lum::fmt