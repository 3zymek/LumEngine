#include "core/editor_console.hpp"
#include "core/utils/fonts.hpp"

namespace lum::editor {

	void Console::Update( ) {

		mNumDebugLogs = 0;
		mNumInfoLogs = 0;
		mNumWarnLogs = 0;
		mNumErrorLogs = 0;

		ImGui::Begin( "Console" );

		ImGuiDockNode* node = ImGui::GetWindowDockNode( );
		if (node) {
			node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
		}

		const float32 inputHeight = ImGui::GetFrameHeightWithSpacing( );
		float32 availableHeight = 0.0f;
		if(bFilterEnabled)
			availableHeight = ImGui::GetContentRegionAvail( ).y - inputHeight;

		ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 6.0f );
		ImGui::PushStyleVar( ImGuiStyleVar_ChildBorderSize, 0.1f );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0, 6.0f ) );
		ImGui::BeginChild( "##LumEngineConsole", ImVec2( -68, availableHeight ), true );
		ImGui::PopStyleVar( 3 );

		ImGui::Indent( 8.0f );
		ImGui::PushFont( Fonts::sConsole );

		auto& logs = Logger::Get( ).GetLogs( );
		for (auto& log : logs) {

			if (bDrawTime)
				draw_timestamp( log );

			if (log.mSeverity == LogSeverity::Debug)
				mNumDebugLogs++;
			else if (log.mSeverity == LogSeverity::Info)
				mNumInfoLogs++;
			else if (log.mSeverity == LogSeverity::Warn)
				mNumWarnLogs++;
			else if (log.mSeverity == LogSeverity::Error)
				mNumErrorLogs++;

			if (!mSeverity.Has( log.mSeverity )) continue;

			auto it = std::search(
				log.mMessage.begin( ), log.mMessage.end( ),
				mFilter.begin( ), mFilter.end( ),
				[]( char a, char b ) { return tolower( a ) == tolower( b ); }
			);

			if (it == log.mMessage.end() && bFilterEnabled) continue;

			ImVec4 color = sSeverityColors[ ToUnderlyingEnum( log.mSeverity ) ];
			ImGui::TextColored( color, "%s", log.mMessage.data( ) );

		}
		ImGui::PopFont( );
		ImGui::Unindent( 8.0f );

		if (ImGui::GetScrollY( ) >= ImGui::GetScrollMaxY( ))
			ImGui::SetScrollHereY( 1.0f );

		ImGui::EndChild( );

		ImGui::SameLine( );

		draw_sidebar( );

		handle_word_filtering( );

		ImGui::End( );

		if (bClearLogs) {
			Logger::Get( ).ClearLogs( );
			bClearLogs = false;
		}

	}

	void Console::draw_timestamp( const FLogEntry& log ) {

		char time[ 10 ]{};
		Logger::FormatTime( log.mTime, time );

		ImGui::TextDisabled( "[ " ); ImGui::SameLine( );
		ImGui::TextDisabled( time ); ImGui::SameLine( );
		ImGui::TextDisabled( " ]" ); ImGui::SameLine( );

	}
	void Console::draw_sidebar( ) {

		const float32 inputHeight = ImGui::GetFrameHeightWithSpacing( );

		ImGui::BeginChild( "##ConsoleSidebar", ImVec2( skSideBarWidth, -inputHeight ), false, ImGuiWindowFlags_NoBackground );

		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0, 0, 0, 0 ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.2f, 0.2f, 0.2f, 1.0f ) );

		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );
		handle_eraser( );
		ImGui::SameLine( );
		handle_copy( );
		ImGui::Separator( );
		handle_search( );
		ImGui::PopStyleVar( );

		ImGui::PopStyleColor( 2 );

		ImGui::Separator( );

		handle_filter( ICON_FA_BUG, mNumDebugLogs, LogSeverity::Debug );
		handle_filter( ICON_FA_INFO_CIRCLE, mNumInfoLogs, LogSeverity::Info );

		ImGui::PushStyleColor( ImGuiCol_Text, sSeverityColors[ ToUnderlyingEnum( LogSeverity::Warn ) ] );
		handle_filter( ICON_FA_EXCLAMATION_TRIANGLE, mNumWarnLogs, LogSeverity::Warn );
		ImGui::PopStyleColor( 1 );

		ImGui::PushStyleColor( ImGuiCol_Text, sSeverityColors[ ToUnderlyingEnum( LogSeverity::Error ) ] );
		handle_filter( ICON_FA_EXCLAMATION_CIRCLE, mNumErrorLogs, LogSeverity::Error );
		ImGui::PopStyleColor( 1 );

		ImGui::EndChild( ); // ##ConsoleSidebar

	}
	void Console::handle_word_filtering( ) {

		if (!bFilterEnabled) return;
		ImGui::SetNextItemWidth( ImGui::GetContentRegionAvail( ).x - skSideBarWidth - ImGui::GetStyle( ).ItemSpacing.x );
		ImGui::InputTextWithHint( "##Filter", "Filter logs...", mFilter.Data( ), mFilter.MaxSize( ) );

	}
	void Console::handle_eraser( ) {

		if (ImGui::Button( ICON_FA_ERASER, ImVec2( skSideBarWidth / 2, 30 ) )) {
			bClearLogs = true;
			mActionTooltip.Trigger( 1.0f );
		}
		mActionTooltip.Draw( "Cleared console" );

	}
	void Console::handle_copy( ) {

		if (ImGui::Button( ICON_FA_CLONE, ImVec2( skSideBarWidth / 2, 30 ) )) {
			String all;
			for (auto& log : Logger::Get( ).GetLogs( )) {
				all += log.mMessage + "\n";
			}
			ImGui::SetClipboardText( all.c_str( ) );
			mActionTooltip.Trigger( 1.0f );
		}
		mActionTooltip.Draw( "Copied to clipboard" );

	}
	void Console::handle_search( ) {

		if (ImGui::Button( ICON_FA_SEARCH, ImVec2( skSideBarWidth / 2, 30 ) )) {

			bFilterEnabled = !bFilterEnabled;
			mActionTooltip.Trigger( );

		}
		//mActionTooltip.Draw( "Word filtering enabled" );

	}

	void Console::handle_filter( StringView icon, uint32 numLogs, LogSeverity sev ) {

		static char buff[ 32 ]{};
		snprintf( buff, sizeof( buff ), "%s  %d", icon.data( ), numLogs );

		ImVec4 color = ImVec4( 1.0f, 1.0f, 1.0f, 0.05f );
		if (!mSeverity.Has( sev )) color = ImVec4( 1.0f, 1.0f, 1.0f, 0.01f );

		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1.0f );
		ImGui::PushStyleColor( ImGuiCol_Button, color );
		ImGui::PushStyleColor( ImGuiCol_Border, ImVec4( 1.0f, 1.0f, 1.0f, 0.1f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.2f, 0.2f, 0.2f, 1.0f ) );

		if (ImGui::Button( buff, ImVec2( skSideBarWidth, 30 ) )) {
			if (mSeverity.Has( sev ))
				mSeverity.Disable( sev );
			else mSeverity.Enable( sev );
		}

		ImGui::PopStyleColor( 3 );
		ImGui::PopStyleVar( 1 );

	}

}