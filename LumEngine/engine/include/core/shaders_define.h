#pragma once

///////////////////////////////////////////
// Shaders locations
///////////////////////////////////////////

// Shader location for position
#define LUM_LAYOUT_POSITION 0

// Shader location for color
#define LUM_LAYOUT_COLOR 1

// Shader location for uv
#define LUM_LAYOUT_UV 2

// Shader location for normal
#define LUM_LAYOUT_NORMAL 3


///////////////////////////////////////////
// Uniform buffers
///////////////////////////////////////////

// Uniform buffer binding for camera ( mat4 uProjection, mat4 uView );
#define LUM_UBO_CAMERA_BINDING 1

// Uniform buffer binding for camera ( mat4 uModel );
#define LUM_UBO_MODEL_BINDING 2


#define LUM_TEXTURE_BINDING_01 1


// Cubemap Texture

#define LUM_CUBEMAP_POSITIVE_X 0
#define LUM_CUBEMAP_NEGATIVE_X 1
#define LUM_CUBEMAP_POSITIVE_Y 2
#define LUM_CUBEMAP_NEGATIVE_Y 3
#define LUM_CUBEMAP_POSITIVE_Z 4
#define LUM_CUBEMAP_NEGATIVE_Z 5
#define LUM_CUBEMAP_BINDING 3


#define LUM_NULL_BINDING max_val<uint16_t>()

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