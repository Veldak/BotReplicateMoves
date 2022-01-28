#include "pch.h"
#include "BotReplicateMoves.h"


BAKKESMOD_PLUGIN(BotReplicateMoves, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void BotReplicateMoves::onLoad()
{
	_globalCvarManager = cvarManager;

	//gameWrapper->HookEvent("Function ProjectX.PlayerInput_X.GetKeyForActionArray", std::bind(&BotReplicateMoves::onTick, this, std::placeholders::_1));

	//gameWrapper->HookEventPost("Function ProjectX.PlayerInput_X.GetKeyForActionArray", std::bind(&BotReplicateMoves::onTick, this, std::placeholders::_1));

	//gameWrapper->HookEvent("Function TAGame.PlayerInput_TA.GetKeyForAction", std::bind(&BotReplicateMoves::onTick, this, std::placeholders::_1));

	gameWrapper->HookEventPost("Function TAGame.Car_TA.SetVehicleInput", std::bind(&BotReplicateMoves::onTick, this, std::placeholders::_1));

	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.Active.StartRound", std::bind(&BotReplicateMoves::InitGame, this, std::placeholders::_1));

	cvarManager->registerNotifier("replicatemoves", [&](std::vector<std::string> args)
		{
			ServerWrapper sw = gameWrapper->GetCurrentGameState();
			if (sw.IsNull()) { return; }

			BallWrapper ball = sw.GetBall();
			ball.SetLocation(Vector{ 0, 0, 92.739998 });
			ball.SetRotation(Rotator{ 0, 0, 0 });
			ball.SetVelocity(Vector{ 0, 0, 0 });

		}, "", 0);


	cvarManager->registerNotifier("replicatemoves_startrecording", [&](std::vector<std::string> args)
		{
			recording = true;

		}, "", 0);
	cvarManager->registerNotifier("replicatemoves_stoprecording", [&](std::vector<std::string> args)
		{
			recording = false;

		}, "", 0);
	cvarManager->registerNotifier("replicatemoves_playrecord", [&](std::vector<std::string> args)
		{
			tickCount = 0;
			inputsIndex = 0;
			botTeleported = false;
			playRecord = true;

		}, "", 0);
	cvarManager->registerNotifier("replicatemoves_stopplaying", [&](std::vector<std::string> args)
		{
			playRecord = false;

		}, "", 0);
}


void BotReplicateMoves::onTick(std::string eventName)
{
	if (!activatePlugin) { return; }

	if (gameWrapper->IsInFreeplay() || gameWrapper->IsInCustomTraining() || gameWrapper->IsInGame())
	{
		ServerWrapper sw = gameWrapper->GetCurrentGameState();
		if (sw.IsNull()) { return; }

		//if I don't do this the grame crashes at the end of the match (error : Access violation reading location 0x0000000000000000, cause the pri below(in the for loop))
		if (sw.GetbMatchEnded())
		{
			return;
		}


		static bool wasRecording = false;

		if (recording)
		{
			CarWrapper car = gameWrapper->GetLocalCar();
			if (!car) { return; }

			if (!wasRecording)
			{
				cvarManager->log("RecordedInputs cleared");
				cvarManager->log("Recording...");

				RecordedInputs.clear();
				RecordedLocations.clear();
				RecordedRotations.clear();
				RecordedVelocities.clear();

				RecordedBallLocations.clear();
				RecordedBallRotations.clear();
				RecordedBallVelocities.clear();

				wasRecording = true;
				recordInitLocation = car.GetLocation();
				recordInitRotation = car.GetRotation();
				recordInitVelocity = car.GetVelocity();
				BoostWrapper carBoost = car.GetBoostComponent();
				if (!carBoost.IsNull())
				{
					recordInitBoostAmount = carBoost.GetCurrentBoostAmount();
				}
				cvarManager->log("car body : " + std::to_string(car.GetLoadoutBody()));
			}


			ControllerInput inputs = car.GetInput();
			RecordedInputs.push_back(inputs);
			RecordedLocations.push_back(car.GetLocation());
			RecordedRotations.push_back(car.GetRotation());
			RecordedVelocities.push_back(car.GetVelocity());
			
			BallWrapper ball = sw.GetBall();
			RecordedBallLocations.push_back(ball.GetLocation());
			RecordedBallRotations.push_back(ball.GetRotation());
			RecordedBallVelocities.push_back(ball.GetVelocity());
		}
		else
		{ 
			wasRecording = false;
		}





		if (playRecord && !recording && RecordedInputs.size() > 0)
		{
			if (botSpawned && botTeleported && inputsIndex < RecordedInputs.size()) //Set recorded inputs to the bot
			{
				CarWrapper botCar = sw.GetPRIs().Get(sw.GetPRIs().Count() - 1).GetCar();
				if (!botCar.IsNull())
				{
					botCar.SetLocation(RecordedLocations.at(inputsIndex));
					botCar.SetRotation(RecordedRotations.at(inputsIndex));
					botCar.SetVelocity(RecordedVelocities.at(inputsIndex));
					botCar.SetInput(RecordedInputs.at(inputsIndex));
				}

				BallWrapper ball = sw.GetBall();
				if (!ball.IsNull())
				{
					ball.SetLocation(RecordedBallLocations.at(inputsIndex));
					ball.SetRotation(RecordedBallRotations.at(inputsIndex));
					ball.SetVelocity(RecordedBallVelocities.at(inputsIndex));
				}


				inputsIndex++;
			}
			else if (botSpawned && !botTeleported && tickCount > 200) //after 200 ticks, bot gets teleported
			{
				CarWrapper botCar = sw.GetPRIs().Get(sw.GetPRIs().Count() - 1).GetCar();

				botCar.GetAIController().DoNothing();
				botCar.SetRotation(recordInitRotation);
				botCar.SetLocation(recordInitLocation);
				//botCar.SetVelocity(recordInitVelocity);
				BoostWrapper BoostBot = botCar.GetBoostComponent();
				if (!BoostBot.IsNull())
				{
					BoostBot.SetBoostAmount(recordInitBoostAmount);
				}
				cvarManager->log("car body : " + std::to_string(botCar.GetLoadoutBody()));


				cvarManager->log("bot teleported !");
				botTeleported = true;
				inputsIndex = 0;
			}
			else if (inputsIndex == RecordedInputs.size())
			{
				playRecord = false;
			}
			else if (!botSpawned) //spawns bot
			{
				std::string botname = "ReplicatingMyMoves";
				sw.SpawnBot(4284, botname);
				cvarManager->log("bot spawned : " + botname);
				botSpawned = true;
			}
			tickCount++;
		}



		/*
		int PlayersCount = 0;

		for (int i = 0; i < pris.Count(); i++)
		{
			PriWrapper pri = pris.Get(i);
			if (!pri.IsNull())
			{
				if (pri.GetbBot())
				{
					CarWrapper carBot = pri.GetCar();
					if (!carBot.IsNull())
					{
						CarWrapper car = gameWrapper->GetLocalCar();
						if (!car.IsNull())
						{
							ControllerInput newInput = car.GetInput();

							carBot.SetInput(newInput);

							BoostWrapper carBoost = car.GetBoostComponent();
							if (!carBoost.IsNull())
							{
								float boostAmount = carBoost.GetCurrentBoostAmount();

								BoostWrapper BoostBot = carBot.GetBoostComponent();
								if (!BoostBot.IsNull())
								{
									BoostBot.SetBoostAmount(boostAmount);
								}
							}
						}
					}
				}
				else
				{
					PlayersCount++;
				}
			}
		}*/

	}
}

void BotReplicateMoves::InitGame(std::string eventName)
{
	ServerWrapper sw = gameWrapper->GetCurrentGameState();
	if (sw.IsNull()) { return; }

	if (sw.GetGameTimeRemaining() != 300.f) { return; }

	cvarManager->log("game started");

	botSpawned = false;

}


void BotReplicateMoves::onUnload()
{
}