//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Parser for .lmt material format files.
//
//=============================================================================//
#pragma once
#include "render/material.hpp"

namespace lum::fmt {

    class Tokenizer;

    /* @brief Parses a tokenized .lmt file into a material descriptor.
    * Used as the second stage of the .lmt parsing pipeline after tokenization.
    */
    class MaterialParser {
    public:
       
       /* @brief Constructs the parser with a reference to an existing tokenizer.
        * @param tokenizer Tokenizer that has already processed the .lmt file content.
        */
        MaterialParser( Tokenizer& tokenizer ) : mTokenizer( tokenizer ) {}

       /* @brief Parses the token stream and fills the material descriptor.
        * @param material Output descriptor to fill with parsed material data.
        */
        void Parse( FMaterialDescriptor& material );

    private:

        Tokenizer& mTokenizer;
    
    };

} // namespace lum::fmt