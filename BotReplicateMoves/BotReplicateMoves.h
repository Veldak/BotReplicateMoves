#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


struct Record
{
	ControllerInput Input;
	Vector Location;
	Rotator Rotation;
	Vector Velocity;

	Vector BallLocation;
	Rotator BallRotation;
	Vector BallVelocity;
};

struct MyStruct
{
	int a;
	bool b;
	std::string c;
};

//magic macro that defines serialize\deserialize functions automagically
//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ControllerInput, Throttle, Steer, Pitch, Yaw, Roll, DodgeForward, DodgeStrafe, Handbrake, Jump, ActivateBoost, HoldingBoost, Jumped)
//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector, X, Y, Z)
//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Rotator, Pitch, Yaw, Roll)
//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Record, Input, Location, Rotation, Velocity, BallLocation, BallRotation, BallVelocity)

void to_json(json& j, const Record& r)
{
	j = json{
		{ "Input", {
			{ "Steer", r.Input.Steer },
			{ "Pitch", r.Input.Pitch },
			{ "Yaw", r.Input.Yaw },
			{ "Roll", r.Input.Roll },
			{ "DodgeForward", r.Input.DodgeForward },
			{ "Handbrake", r.Input.Handbrake },
			{ "Jump", r.Input.Jump },
			{ "ActivateBoost", r.Input.ActivateBoost },
			{ "HoldingBoost", r.Input.HoldingBoost },
			{ "Jumped", r.Input.Jumped },
			{ "DodgeStrafe ", r.Input.DodgeStrafe }
			}
		},
		{ "Location", {
			{ "X", r.Location.X },
			{ "Y ", r.Location.Y },
			{ "Z", r.Location.Z },
			}
		},
		{ "Rotation", {
			{ "Pitch", r.Rotation.Pitch },
			{ "Yaw ", r.Rotation.Yaw },
			{ "Roll", r.Rotation.Roll },
			}
		},
		{ "Velocity", {
			{ "X", r.Velocity.X },
			{ "Y ", r.Velocity.Y },
			{ "Z", r.Velocity.Z },
			}
		},
		{ "BallLocation", {
			{ "X", r.BallLocation.X },
			{ "Y ", r.BallLocation.Y },
			{ "Z", r.BallLocation.Z },
			}
		},
		{ "BallRotation", {
			{ "Pitch", r.BallRotation.Pitch },
			{ "Yaw ", r.BallRotation.Yaw },
			{ "Roll", r.BallRotation.Roll },
			}
		},
		{ "BallVelocity", {
			{ "X", r.BallVelocity.X },
			{ "Y ", r.BallVelocity.Y },
			{ "Z", r.BallVelocity.Z },
			}
		}
	};
}

void from_json(const json& j, Record& record)
{
	j.at("Input").at("Steer").get_to(record.Input.Steer);
	j.at("Input").at("Pitch").get_to(record.Input.Pitch);
	j.at("Input").at("Yaw").get_to(record.Input.Yaw);
	j.at("Input").at("Roll").get_to(record.Input.Roll);
	j.at("Input").at("DodgeForward").get_to(record.Input.DodgeForward);
	j.at("Input").at("Handbrake").get_to(record.Input.Handbrake);
	j.at("Input").at("Jump").get_to(record.Input.Jump);
	j.at("Input").at("ActivateBoost").get_to(record.Input.ActivateBoost);
	j.at("Input").at("Steer").get_to(record.Input.Steer);
	j.at("Input").at("Steer").get_to(record.Input.Steer);
	j.at("Input").at("Steer").get_to(record.Input.Steer);
	j.at("last").get_to(name.last);
}



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

