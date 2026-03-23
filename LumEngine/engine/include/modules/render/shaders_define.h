//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Defines which gets pasted into every shader
//
//=============================================================================//
#pragma once

/* @brief Pi constant for use in lighting calculations. */
#define LUM_PI 3.14159265359

// Vertex attribute locations
#define LUM_LAYOUT_POSITION  0 // Vertex position attribute location.
#define LUM_LAYOUT_NORMAL    1 // Vertex normal attribute location.
#define LUM_LAYOUT_UV        2 // Vertex UV attribute location.
#define LUM_LAYOUT_TANGENT   3 // Vertex tangent attribute location.
#define LUM_LAYOUT_BITANGENT 4 // Vertex bitangent attribute location.

// Texture binding slots - geometry pass
#define LUM_TEX_ALBEDO    0 // Albedo texture binding.
#define LUM_TEX_NORMAL    1 // Normal map binding.
#define LUM_TEX_ROUGHNESS 2 // Roughness map binding.
#define LUM_TEX_METALNESS 3 // Metalness map binding.
#define LUM_TEX_CUBEMAP   4 // Cubemap texture binding.

// GBuffer sampler bindings - light pass
#define LUM_GBUFFER_ALBEDO  0 // Albedo GBuffer sampler binding.
#define LUM_GBUFFER_NORMAL  1 // Normal GBuffer sampler binding.
#define LUM_GBUFFER_DEPTH   2 // Depth GBuffer sampler binding.

<<<<<<< HEAD
#define LUM_TEX_OUTPUT 2

=======
>>>>>>> f8ece7c (Editor in works)
#define LUM_SHADOW_MAP 6

// Uniform buffer bindings
#define LUM_UBO_CAMERA_BINDING     0 // Camera matrices and position.
#define LUM_UBO_MODEL_BINDING      1 // Model matrix.
#define LUM_UBO_MATERIAL_BINDING   2 // Material parameters.
#define LUM_UBO_DIRECTIONAL_LIGHT  3 // Directional light data.
#define LUM_UBO_LIGHTSPACE_MATRIX  10

// Shader storage buffer bindings
#define LUM_SSBO_LIGHTS_BINDING 0 // Point light array.

/* @brief Maximum number of point lights active in a single frame. */
#define LUM_MAX_LIGHTS 64

// Cubemap face indices
#define LUM_CUBEMAP_POSITIVE_X 0 // +X face.
#define LUM_CUBEMAP_NEGATIVE_X 1 // -X face.
#define LUM_CUBEMAP_POSITIVE_Y 2 // +Y face.
#define LUM_CUBEMAP_NEGATIVE_Y 3 // -Y face.
#define LUM_CUBEMAP_POSITIVE_Z 4 // +Z face.
#define LUM_CUBEMAP_NEGATIVE_Z 5 // -Z face.