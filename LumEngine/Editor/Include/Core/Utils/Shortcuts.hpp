//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Keyboard shortcut helper utilities.
//
//=============================================================================//
#pragma once

#include "Platform/InputCommon.hpp"
#include "Event/EventBus.hpp"
#include "Event/Events/WindowEvents.hpp"

namespace lum::editor {

	/* @brief Registers a keyboard shortcut callback.
	*
	*  The callback is executed when the modifier key is held
	*  and one of the action keys is pressed.
	*
	*  @tparam tModifier Modifier key that must be held.
	*  @tparam tActionKeys Keys that activate the shortcut.
	*  @tparam tLambda Callback callable type.
	*
	*  @param evBus Event bus used for keyboard event subscription.
	*  @param callback Function invoked when the shortcut is triggered.
	*/
	template<input::Key tModifier, input::Key... tActionKeys, typename tLambda>
	void AddShortcut( ev::EventBus* evBus, tLambda&& callback ) {

		evBus->SubscribePermanently<EKeyPressed>(
			[lambda = std::forward<tLambda>( callback )]( const EKeyPressed& key ) {

				if (input::KeyPressed( tModifier ) && ((key.mKey == tActionKeys) || ...)) {

					lambda( );
				}

			}
		);

	}

} // namespace lum::editor