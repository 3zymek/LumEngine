//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Keyboard input abstraction layer for GLFW
// 
//=============================================================================//
#pragma once

namespace lum::input {

	/* @brief Unified virtual key codes for keyboard and mouse input handling.
	*
	* Maps platform-independent input layout (matching GLFW/Native virtual key standards)
	* into a single enum class used by InputSystem and event dispatchers.
	*/
	enum class Key {

		Space,
		Apostrophe, Comma, Minus, Period, Slash,
		Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
		Semicolon, Equal,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		LeftBracket, Backslash, RightBracket,
		GraveAccent, /* @brief Backtick / tilde key (` / ~) placed below Escape. */

		/* @brief Non-US / international layout keys (ISO keyboards).
		* Used for OEM specific keys present on non-US keyboard layouts.
		*/
		World1,
		World2,

		Escape, Enter, Tab, Backspace, Insert, DeleteKey,
		Right, Left, Down, Up,
		PageUp, PageDown, Home, End,
		CapsLock, ScrollLock, NumLock, PrintScreen, Pause,

		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,

		/* @brief Numeric Keypad (Numpad) keys. */
		Kp0, Kp1, Kp2, Kp3, Kp4, Kp5, Kp6, Kp7, Kp8, Kp9,
		KpDecimal, KpDivide, KpMultiply, KpSubtract, KpAdd, KpEnter, KpEqual,

		/* @brief Modifier keys. */
		LeftShift, LeftControl, LeftAlt,
		LeftSuper,        /* @brief Left Windows key on PC, Command key on macOS, Super key on Linux. */
		RightShift, RightControl, RightAlt,
		RightSuper,       /* @brief Right Windows key on PC, Command key on macOS, Super key on Linux. */
		Menu,             /* @brief Context menu key usually placed next to Right Control. */

		/* @brief Mouse button virtual keys mapped into unified input stream. */
		MouseLeft,
		MouseRight,
		MouseMiddle,

		/* @brief Total count of valid virtual key entries.
		* Useful for sizing fixed arrays storing key states (down/pressed/up).
		*/
		MaxCount
	};

	/* @brief Total number of virtual key code entries available in the Key enum. */
#	define LUM_MAX_KEYS ToUnderlyingEnum( Key::MaxCount )

	/* @brief Compile-time assertion ensuring input mapping array size matches Key::MaxCount precisely.
	* @param size Size of the platform-specific key map array (e.g., s_KeyMap.size()).
	*/
#	define LUM_ASSERT_KEY_ARRAY_SIZE( size ) \
		LUM_SASSERT( size == static_cast<int32>(lum::input::Key::MaxCount) && "Key map array size mismatch!" )

} // namespace lum::input