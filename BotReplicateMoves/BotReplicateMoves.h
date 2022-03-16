#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


struct Record
{
	MyControllerInput Input;
	Vector Location;
	Rotator Rotation;
	Vector Velocity;

	Vector BallLocation;
	Rotator BallRotation;
	Vector BallVelocity;
};

struct MyControllerInput
{
	float Throttle = .0f;
	float Steer = .0f;
	float Pitch = .0f;
	float Yaw = .0f;
	float Roll = .0f;
	float DodgeForward = .0f;
	float DodgeStrafe = .0f;
	unsigned long Handbrake;
	unsigned long Jump;
	unsigned long ActivateBoost;
	unsigned long HoldingBoost;
	unsigned long Jumped;
};


//magic macro that defines serialize\deserialize functions automagically
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MyControllerInput, Throttle, Steer, Pitch, Yaw, Roll, DodgeForward, DodgeStrafe, Handbrake, Jump, ActivateBoost, HoldingBoost, Jumped)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector, X, Y, Z)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Rotator, Pitch, Yaw, Roll)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Record, Input, Location, Rotation, Velocity, BallLocation, BallRotation, BallVelocity)


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

	std::vector<Record> RecordsList;


	void SaveActualRecord(std::vector<Record> recordsList);


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

