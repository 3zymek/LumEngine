//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: LumEngine setup defines
// 
//=============================================================================//
#pragma once

// Defines if logs can be dispached.
#define LUM_ENABLE_LOGGER 1
#ifndef LUM_ENABLE_LOGGER
#	error "LUM_ENABLE_LOGGER should be defined"
#endif

// Defines if debug logging in backend rendering API is used.
#define LUM_ENABLE_DEBUG_RENDER 0
#ifndef LUM_ENABLE_DEBUG_RENDER
#	error "LUM_ENABLE_DEBUG_RENDER should be defined"
#endif

// Defines if render hardware interface profile is enabled
#define LUM_ENABLE_RENDER_PROFILER 1
#ifndef LUM_ENABLE_RENDER_PROFILER
#	error "LUM_ENABLE_RENDER_PROFILER should be defined"
#endif

// Defines if validation statements are used in hotpath ( render, ecs, etc... )
#define LUM_ENABLE_HOTPATH_VALIDATION 1
#ifndef LUM_ENABLE_HOTPATH_VALIDATION
#	error "LUM_ENABLE_HOTPATH_VALIDATION should be defined"
#endif