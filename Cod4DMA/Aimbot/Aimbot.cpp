#include "Pch.h"
#include "aimbot.h"
#include "PlayerEsp.h"
#include "globals.h"
#include "CheatFunction.h"
#include "Drawing.h"
#include "ConfigInstance.h"
#include "ConfigUtilities.h"
#include "Kmbox.h"
#include "InputManager.h"
#include "Game.h"
#include "Player.h"


int ConditionalSwapPlayer(std::vector<std::shared_ptr<Player>>& entities, int low, int high)
{
	std::shared_ptr<Player> pivot = entities[high];
	int i = low - 1;
	Vector2 centreofscreen = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height * 0.6f : GetSystemMetrics(SM_CYSCREEN) * 0.6f);
	for (int j = low; j < high; ++j)
	{
		if (Configs.Aimbot.Priority == 2)
		{
			if (Vector2::Distance(centreofscreen, GameInstance->GetLocalPlayer()->WorldToScreen(entities[j]->GetAimBonePosition())) < Vector2::Distance(centreofscreen, GameInstance->GetLocalPlayer()->WorldToScreen(pivot->GetAimBonePosition())))
			{
				++i;
				std::swap(entities[i], entities[j]);
				continue;
			}
			if (Vector3::SubDistance(GameInstance->GetLocalPlayer()->GetPosition(), entities[j]->GetAimBonePosition()) < Vector3::SubDistance(GameInstance->GetLocalPlayer()->GetPosition(), pivot->GetAimBonePosition()))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
		if (Configs.Aimbot.Priority == 0)
		{
			if (Vector3::SubDistance(GameInstance->GetLocalPlayer()->GetPosition(), entities[j]->GetAimBonePosition()) < Vector3::SubDistance(GameInstance->GetLocalPlayer()->GetPosition(), pivot->GetAimBonePosition()))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
		if (Configs.Aimbot.Priority == 1)
		{
			if (Vector2::Distance(centreofscreen, GameInstance->GetLocalPlayer()->WorldToScreen(entities[j]->GetAimBonePosition())) < Vector2::Distance(centreofscreen, GameInstance->GetLocalPlayer()->WorldToScreen(pivot->GetAimBonePosition())))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
	}
	std::swap(entities[i + 1], entities[high]);
	return i + 1;
}

void QuickSortPlayers(std::vector<std::shared_ptr<Player>>& entities, int low, int high)
{
	if (low < high)
	{
		int pi = ConditionalSwapPlayer(entities, low, high);
		QuickSortPlayers(entities, low, pi - 1);
		QuickSortPlayers(entities, pi + 1, high);
	}
}

std::shared_ptr<Player> AimbotTarget;

void GetAimbotTarget()
{
	if (GameInstance->GetLocalPlayer() == nullptr)
		return;
	if (GameInstance->GetPlayerCount() == 0)
		return;
	if (GameInstance->GetPlayers().size() == 0)
		return;
	if (GameInstance->GetLocalPlayer()->GetPosition() == Vector3::Zero())
		return;
	if (!Configs.Aimbot.Enable)
		return;
	Vector2 centreofscreen = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height /2 : GetSystemMetrics(SM_CYSCREEN) /2);

	std::vector<std::shared_ptr<Player>> templist;
	Vector3 localpos = GameInstance->GetLocalPlayer()->GetPosition();
	
		templist = GameInstance->GetPlayers();

	QuickSortPlayers(templist, 0, templist.size() - 1);

	for (std::shared_ptr<Player> player : templist)
	{
		if (player == nullptr)
			continue;
		if (!Configs.Aimbot.TargetPlayers)
			continue;
		if (!player->GetClientSize())
			continue;
		if (!player->GetAlive())
			continue;
		if (Vector3::SubDistance(GameInstance->GetLocalPlayer()->GetPosition(), player->GetPosition()) > Configs.Aimbot.MaxDistance)
			continue;
		if (GameInstance->GetLocalPlayer()->WorldToScreen(player->GetAimBonePosition()) == Vector2::Zero())
			continue;
		if (Vector2::Distance(GameInstance->GetLocalPlayer()->WorldToScreen(player->GetAimBonePosition()), centreofscreen) > Configs.Aimbot.FOV)
			continue;
		AimbotTarget = player;
		//	printf("Targeting: %s\n", AimbotTarget->GetName().c_str());
		return;
	}
	AimbotTarget = nullptr;
}

bool AimKeyDown = false;
std::shared_ptr<CheatFunction> UpdateAimKey = std::make_shared<CheatFunction>(50, [] {
	if (GameInstance->GetLocalPlayer() == nullptr)
		return;
	if (GameInstance->GetPlayerCount() == 0)
		return;
	if (GameInstance->GetPlayers().size() == 0)
		return;
	if (GameInstance->GetLocalPlayer()->GetPosition() == Vector3::Zero())
		return;
	if (!Configs.Aimbot.Enable)
		return;
	if (Keyboard::IsKeyDown(Configs.Aimbot.Aimkey))
	{
		AimKeyDown = true;
	}
	else
	{
		AimKeyDown = false;
	}
	});

std::chrono::system_clock::time_point KmboxStart;
void Aimbot()
{
	UpdateAimKey->Execute();
	if (!kmbox::connected || !AimKeyDown)
	{
		AimbotTarget = nullptr;
		return;
	}
	GetAimbotTarget();
	if (AimbotTarget == nullptr)
		return;

	if (AimbotTarget->GetPosition() == Vector3::Zero())
	{
		AimbotTarget = nullptr;
		return;
	}
	Vector2 screenpos = GameInstance->GetLocalPlayer()->WorldToScreen(AimbotTarget->GetAimBonePosition());
	Vector2 centreofscreen = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height /2 : GetSystemMetrics(SM_CYSCREEN)/2);
	if (Vector2::Distance(screenpos, centreofscreen) > Configs.Aimbot.FOV)
		return;
	if (screenpos == Vector2::Zero())
	{
		AimbotTarget = nullptr;
		return;
	}
	Vector2 diff;
	if (Configs.Aimbot.Smoothing > 0)
	{
		float x = screenpos.x - centreofscreen.x;
		float y = screenpos.y - centreofscreen.y;
		int smoothx = Configs.Aimbot.Smoothing;
		int smoothy = Configs.Aimbot.Smoothing;
		float randomnumber = (float)(rand() % 100001) / 100000;
		srand(__rdtsc());
		diff.x = static_cast<int>(std::round(x / ((smoothx > 1 ? smoothx / 2 : smoothx)
			+ (float(rand() % ((int)(smoothx > 1 ? smoothx / 2 : smoothx)) + (float)(rand() % 5) + randomnumber)))));
		diff.y = static_cast<int>(std::round(y / ((smoothy > 1 ? smoothy / 2 : smoothy)
			+ (float(rand() % ((int)(smoothy > 1 ? smoothy / 2 : smoothy)) + (float)(rand() % 5) + randomnumber)))));
	
	}
	else
	{
		diff.x = screenpos.x - centreofscreen.x;
		diff.y = screenpos.y - centreofscreen.y;
	}
	float x = diff.x;
	float y = diff.y;

	if (KmboxStart + std::chrono::milliseconds(15) < std::chrono::system_clock::now())
	{
		kmbox::move(x, y);
		KmboxStart = std::chrono::system_clock::now();
	}

}
