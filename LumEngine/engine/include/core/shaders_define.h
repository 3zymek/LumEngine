//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Defines which gets pasted into every shader
// 
//=============================================================================//
#pragma once

// Shader location for position
#define LUM_LAYOUT_POSITION 0

// Shader location for normal
#define LUM_LAYOUT_NORMAL 1

// Shader location for uv
#define LUM_LAYOUT_UV 2

// Uniform buffer binding for camera matrices ( projection and view ).
#define LUM_UBO_CAMERA_BINDING 1

// Uniform buffer binding for model matrix.
#define LUM_UBO_MODEL_BINDING 2

#define LUM_SSBO_LIGHTS_BINDING 3
#define LUM_MAX_LIGHTS 3

// Sampler slots for material textures.
#define LUM_SAMPLER_ALBEDO		0 // Albedo texture sampler.
#define LUM_SAMPLER_NORMAL		1 // Normal map sampler.
#define LUM_SAMPLER_ROUGNESS	2 // Roughness map sampler.
#define LUM_SAMPLER_METALNESS	3 // Metalness map sampler.

// Cubemap sampler binding slot
#define LUM_SAMPLER_CUBEMAP 8 // Sampler slot for cubemap

// Texture slots for material textures.
#define LUM_TEX_ALBEDO		0 // Albedo texture binding.
#define LUM_TEX_NORMAL		1 // Normal map binding.
#define LUM_TEX_ROUGHNESS	2 // Roughness map binding.
#define LUM_TEX_METALNESS	3 // Metalness map binding.

// Cubemap face indices
#define LUM_CUBEMAP_POSITIVE_X 0 // Positive X face texture.
#define LUM_CUBEMAP_NEGATIVE_X 1 // Negative X face texture.
#define LUM_CUBEMAP_POSITIVE_Y 2 // Positive Y face texture.
#define LUM_CUBEMAP_NEGATIVE_Y 3 // Negative Y face texture.
#define LUM_CUBEMAP_POSITIVE_Z 4 // Positive Z face texture.
#define LUM_CUBEMAP_NEGATIVE_Z 5 // Negative Z face texture.

// Cubemap texture binding slot.
#define LUM_TEX_CUBEMAP 8 // Cubemap texture binding.

// Null/default binding value.
#define LUM_NULL_BINDING MaxVal<uint16>() // Invalid/null binding.


///////////////////////////////////////////
// Instance names
///////////////////////////////////////////

#define LUM_CAMERA_PROJECTION_MAT4 uProjection
#define LUM_CAMERA_PROJECTION_MAT4_STR "uProjection"

#define LUM_CAMERA_VIEW_MAT4 uView
#define LUM_CAMERA_VIEW_MAT4_STR "uView"

#define LUM_CAMERA_POSITION uCameraPos
#define LUM_CAMERA_POSITION_STR "uCameraPos"

#define LUM_MODEL_MAT4 uModel
#define LUM_MODEL_MAT4_STR "uModel"