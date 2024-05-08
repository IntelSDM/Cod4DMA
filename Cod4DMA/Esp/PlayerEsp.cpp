#include "Pch.h"
#include "PlayerEsp.h"
#include "globals.h"
#include "CheatFunction.h"
#include "Drawing.h"
#include "ConfigInstance.h"
#include "ConfigUtilities.h"

std::shared_ptr<CheatFunction> UpdatePlayers = std::make_shared<CheatFunction>(10, [] {
	
	GameInstance->UpdatePlayers();	

});

void DrawPlayers()
{
	if(GameInstance->GetLocalPlayer() == nullptr)
		return;
	if(GameInstance->GetPlayerCount() == 0)
		return;
	if(GameInstance->GetPlayers().size() == 0)
		return;
	if(GameInstance->GetLocalPlayer()->GetPosition() == Vector3::Zero())
		return;
	for (auto player : GameInstance->GetPlayers())
	{
		if(player->GetPosition() == Vector3::Zero())
			continue;
		if(player->GetClientSize() == 0)
			continue;
		if(player->GetAlive() == false)
			continue;

		Vector2 pos = GameInstance->GetLocalPlayer()->WorldToScreen(player->GetPosition());
		if(pos == Vector2::Zero())
			continue;
		DrawText(pos.x, pos.y, L"Test", "Verdana", 12, MyColour(255, 255, 255), Centre);
	}
}