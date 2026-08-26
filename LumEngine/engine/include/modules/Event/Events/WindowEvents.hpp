//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Window and input events.
//
//=============================================================================//
#pragma once
#include "Event/EventCommon.hpp"
#include "Platform/Input/Input.hpp"

namespace lum {

	struct EWindowResized	: public Event { uint32 m_Width; uint32 m_Height; };
	struct EWindowClosed	: public Event { };
	struct EKeyPressed		: public Event { input::Key m_Key; };
	struct EKeyReleased		: public Event { input::Key m_Key; };

} // namespace lum