//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Timed tooltip that fades out over a given duration.
//
//=============================================================================//
#pragma once
#include "core/editor_common.hpp"

namespace lum::editor {

	class TimedTooltip {
	public:

		void Trigger( StringView msg, float32 duration = 1.5f ) {
			bActive = true;
			mTimer = duration;
			mText = msg;
		}
		void Draw( ) {
			if (!bActive) return;
			mTimer -= ImGui::GetIO( ).DeltaTime;
			ImGui::PushStyleVar( ImGuiStyleVar_Alpha, mTimer );
			ImGui::SetTooltip( mText.data() );
			ImGui::PopStyleVar( 1 );
			if (mTimer <= 0.0f) bActive = false;
		}

	private:

		bool bActive = false;	// Whether the tooltip is currently active.
		float32 mTimer = 0.0f;	// Remaining time before the tooltip disappears.
		StringView mText;

	};

} // namespace lum::editor