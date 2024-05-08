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
			if (Vector2::Distance(centreofscreen, GameInstance->GetLocalPlayer()->WorldToScreen(entities[j]->GetPosition())) < Vector2::Distance(centreofscreen, GameInstance->GetLocalPlayer()->WorldToScreen(pivot->GetPosition())))
			{
				++i;
				std::swap(entities[i], entities[j]);
				continue;
			}
			if (Vector3::Distance(GameInstance->GetLocalPlayer()->GetPosition(), entities[j]->GetPosition()) < Vector3::Distance(GameInstance->GetLocalPlayer()->GetPosition(), pivot->GetPosition()))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
		if (Configs.Aimbot.Priority == 0)
		{
			if (Vector3::Distance(GameInstance->GetLocalPlayer()->GetPosition(), entities[j]->GetPosition()) < Vector3::Distance(GameInstance->GetLocalPlayer()->GetPosition(), pivot->GetPosition()))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
		if (Configs.Aimbot.Priority == 1)
		{
			if (Vector2::Distance(centreofscreen, GameInstance->GetLocalPlayer()->WorldToScreen(entities[j]->GetPosition())) < Vector2::Distance(centreofscreen, GameInstance->GetLocalPlayer()->WorldToScreen(pivot->GetPosition())))
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
bool StickTarget()
{
	Vector2 centreofscreen = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height /2 : GetSystemMetrics(SM_CYSCREEN) /2);
	if (GameInstance->GetLocalPlayer() == nullptr)
		return;
	if (GameInstance->GetPlayerCount() == 0)
		return;
	if (GameInstance->GetPlayers().size() == 0)
		return;
	if (GameInstance->GetLocalPlayer()->GetPosition() == Vector3::Zero())
		return;
	if (AimbotTarget == nullptr)
		return false;
	if (!AimbotTarget->GetAlive())
		return false;
	if (!AimbotTarget->GetClientSize())
		return false;
	if (Vector3::Distance(GameInstance->GetLocalPlayer()->GetPosition(), AimbotTarget->GetPosition()) > Configs.Aimbot.MaxDistance)
		return false;
	if (GameInstance->GetLocalPlayer()->WorldToScreen(AimbotTarget->GetPosition()) == Vector2::Zero())
		return false;
	if (Vector2::Distance(GameInstance->GetLocalPlayer()->WorldToScreen(AimbotTarget->GetPosition()), centreofscreen) > Configs.Aimbot.FOV)
		return false;
	return true;
}

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
	if (StickTarget())
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
		if (Vector3::Distance(GameInstance->GetLocalPlayer()->GetPosition(), player->GetPosition()) <= 2.0f) // local player
			continue;
		if (Vector3::Distance(GameInstance->GetLocalPlayer()->GetPosition(), player->GetPosition()) > Configs.Aimbot.MaxDistance)
			continue;
		if (GameInstance->GetLocalPlayer()->WorldToScreen(player->GetPosition()) == Vector2::Zero())
			continue;
		if (Vector2::Distance(GameInstance->GetLocalPlayer()->WorldToScreen(player->GetPosition()), centreofscreen) > Configs.Aimbot.FOV)
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
	Vector2 screenpos = GameInstance->GetLocalPlayer()->WorldToScreen(AimbotTarget->GetHeadWorldPosition());
	Vector2 centreofscreen = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height /2 : GetSystemMetrics(SM_CYSCREEN)/2);
	if (Vector2::Distance(screenpos, centreofscreen) > Configs.Aimbot.FOV)
		return;
	if (screenpos == Vector2::Zero())
	{
		AimbotTarget = nullptr;
		return;
	}

	float x = screenpos.x - centreofscreen.x;
	float y = screenpos.y - centreofscreen.y;

	if (KmboxStart + std::chrono::milliseconds(15) < std::chrono::system_clock::now())
	{
		kmbox::move(x, y);
		KmboxStart = std::chrono::system_clock::now();
	}

}
