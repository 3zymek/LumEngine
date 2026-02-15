//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: LumEngine setup defines
// 
//=============================================================================//
#pragma once

// Defines if ImGui library is enabled and can be used.
#define LUM_ENABLE_IMGUI 1

// Defines if logs can be dispached.
#define LUM_ENABLE_LOGGER 1

// Defines if debug logging in backend rendering API is used.
#define LUM_ENABLE_DEBUG_RENDER 1

// Defines if render hardware interface profile is enabled
#define LUM_ENABLE_RENDER_PROFILER 1

// Defines if validation statements are used in hotpath ( render, ecs, etc... )
#define LUM_ENABLE_HOTPATH_VALIDATION 1