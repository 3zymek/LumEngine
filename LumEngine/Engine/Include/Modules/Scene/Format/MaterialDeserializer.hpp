/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once

//#include "Render/Material.hpp"

namespace lum::fmt {

    class Tokenizer;

    class MaterialDeserializer {
    public:

        MaterialDeserializer() = delete;

        /* @brief Deserializes a tokenized .lmt file into a material descriptor.
        * Used as the second stage of the .lmt parsing pipeline after tokenization.
        */
        //static void Deserialize(MaterialDescriptor& desc, Tokenizer& tokenizer);

    private:


    };

} // namespace lum::fmt