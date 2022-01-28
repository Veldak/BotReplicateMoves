#include "pch.h"
#include "BotReplicateMoves.h"


std::string BotReplicateMoves::GetPluginName() {
	return "BotReplicateMoves";
}

void BotReplicateMoves::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> BotReplicateMoves
void BotReplicateMoves::RenderSettings()
{
	ImGui::Checkbox("Enable Plugin", &activatePlugin);

	ImGui::NewLine();

	if (ImGui::Button("Set binds"))
	{
		gameWrapper->Execute([&](GameWrapper* gw) {
			cvarManager->executeCommand("bind XboxTypeS_DPad_Down \"replicatemoves_startrecording\"");
			cvarManager->executeCommand("bind XboxTypeS_DPad_Left \"replicatemoves_stoprecording\"");
			cvarManager->executeCommand("bind XboxTypeS_DPad_Right \"replicatemoves_playrecord\"");
			cvarManager->executeCommand("bind XboxTypeS_DPad_Up \"replicatemoves_stopplaying\"");
			});
	}
	ImGui::SameLine();
	if (ImGui::Button("reset binds"))
	{
		gameWrapper->Execute([&](GameWrapper* gw) {
			cvarManager->executeCommand("bind XboxTypeS_DPad_Down \"default_down\"");
			cvarManager->executeCommand("bind XboxTypeS_DPad_Left \"default_left\"");
			cvarManager->executeCommand("bind XboxTypeS_DPad_Right \"default_right\"");
			cvarManager->executeCommand("bind XboxTypeS_DPad_Up \"default_up\"");
			});
	}

	ImGui::NewLine();

	if (ImGui::Button("Start recording"))
	{
		recording = true;
	}

	if (ImGui::Button("Stop recording"))
	{
		recording = false;
	}

	ImGui::NewLine();


	if (ImGui::Button("Play"))
	{
		tickCount = 0;
		inputsIndex = 0;
		botTeleported = false;
		playRecord = true;
	}

	if (ImGui::Button("Stop Playing"))
	{
		playRecord = false;
	}

	ImGui::Text("RecordedInputs Count : %d", RecordedInputs.size());
}


/*
// Do ImGui rendering here
void BotReplicateMoves::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string BotReplicateMoves::GetMenuName()
{
	return "BotReplicateMoves";
}

// Title to give the menu
std::string BotReplicateMoves::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void BotReplicateMoves::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool BotReplicateMoves::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool BotReplicateMoves::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void BotReplicateMoves::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void BotReplicateMoves::OnClose()
{
	isWindowOpen_ = false;
}
*/
