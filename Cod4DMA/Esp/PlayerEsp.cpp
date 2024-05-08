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
	if(!Configs.Player.Enable)
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
		int distance = Vector3::Distance(player->GetPosition(), GameInstance->GetLocalPlayer()->GetPosition());
		if(distance > Configs.Player.MaxDistance)
			continue;
		Vector2 headpos = GameInstance->GetLocalPlayer()->WorldToScreen(player->GetHeadWorldPosition());
		std::wstring name = Configs.Player.Name ? L"Player" : L"";
		std::wstring wdistance = Configs.Player.Distance ? L"[" + std::to_wstring(distance) + L"m]" : L"";
		DrawText(pos.x, pos.y, name + wdistance, "Verdana", Configs.Player.FontSize, Configs.Player.TextColour, Centre);
		float height = Vector2::Distance(pos, headpos);
		if(Configs.Player.Box)
		OutlineRectangle(headpos.x - (height / 4), headpos.y, height/2, height, 1, Configs.Player.BoxColour);
	}
}