#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class BotReplicateMoves: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow/*, public BakkesMod::Plugin::PluginWindow*/
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();


	bool activatePlugin = true;
	bool recording = false;
	bool playRecord = false;
	bool botSpawned = false;
	bool botTeleported = false;

	Vector recordInitLocation;
	Rotator recordInitRotation;
	Vector recordInitVelocity;
	float recordInitBoostAmount = 1.f;

	int tickCount = 0;
	int inputsIndex = 0;


	std::vector<ControllerInput> RecordedInputs;
	std::vector<Vector> RecordedLocations;
	std::vector<Rotator> RecordedRotations;
	std::vector<Vector> RecordedVelocities;


	std::vector<Vector> RecordedBallLocations;
	std::vector<Rotator> RecordedBallRotations;
	std::vector<Vector> RecordedBallVelocities;


	void onTick(std::string eventName);
	void InitGame(std::string eventName);

	// Inherited via PluginSettingsWindow
	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;
	

	// Inherited via PluginWindow
	/*

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "BotReplicateMoves";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;
	
	*/
};

