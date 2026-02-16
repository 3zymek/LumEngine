//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Sets engine hard limits
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"
#include "core/shaders_define.h"

namespace lum::limits {

	//============================= ECS ==================================
	// Defines how much memory needs to be reserved for components
	LUM_COMPILE_VARIABLE usize gMaxEcsPoolCapacity = 1024;
	LUM_COMPILE_VARIABLE usize gMaxEntity = 1024;

	// Defines how much containers needs to be reserved for components
	LUM_COMPILE_VARIABLE usize gMaxComponents = 32;



	//============================= Audio ==================================
	// Defines how much memory needs to be reserved for audio
	LUM_COMPILE_VARIABLE usize gMaxAudioSounds = 256;
	LUM_COMPILE_VARIABLE usize gMaxAudioChannels = 256;



	//============================= Events ==================================
	// Defines how much memory needs to be reserved for pools
	LUM_COMPILE_VARIABLE usize gMaxEventTypes = 32;

	// All settings under are for single event type

	LUM_COMPILE_VARIABLE usize gMaxCallbackPerFrame = 8;
	LUM_COMPILE_VARIABLE usize gMaxPermanentCallbacks = 8;
	LUM_COMPILE_VARIABLE usize gMaxEventEmittsPerFrame = 64;


	//============================= Render ==================================
	LUM_COMPILE_VARIABLE usize gMaxModels = 256;
	LUM_COMPILE_VARIABLE usize gMaxLights = LUM_MAX_LIGHTS;
	LUM_COMPILE_VARIABLE usize gMaxMaterials = 256;
	LUM_COMPILE_VARIABLE usize gMaxTextures = 256;



}