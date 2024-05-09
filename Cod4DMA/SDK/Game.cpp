#include "Pch.h"
#include "Game.h"

Game::Game()
{
	PlayerCount = TargetProcess.Read<short int>( PlayerCountOffset);
	LocalPlayerInstance = std::make_shared<LocalPlayer>();
}

short int Game::GetPlayerCount()
{
	return PlayerCount;
}

void Game::UpdatePlayerCount(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PlayerCountOffset,reinterpret_cast<void*>(&PlayerCount), sizeof(short int));
}

void Game::Cache()
{
	auto handle = TargetProcess.CreateScatterHandle();
	UpdatePlayerCount(handle);
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	if(PlayerCount == 0)
		return;
	std::vector<std::shared_ptr<Player>> templayers;	
	 handle = TargetProcess.CreateScatterHandle();
	LocalPlayerInstance->UpdateClientIndex(handle);
	LocalPlayerInstance->UpdateFov(handle);
	LocalPlayerInstance->UpdateViewAngles(handle);
	LocalPlayerInstance->UpdatePosition(handle);
	for (int i = 0; i <= PlayerCount+1; i++)
	{
		
		std::shared_ptr<Player> player = std::make_shared<Player>(i);
		player->UpdateClientSize(handle);
		player->UpdateTeam(handle);
		player->UpdateAlive(handle);
		player->UpdatePosition(handle);
		player->UpdateStance(handle);
		templayers.push_back(player);
		
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	LocalPlayerInstance->SetUpPlayerInstance();
	handle = TargetProcess.CreateScatterHandle();
	LocalPlayerInstance->GetPlayerInstance()->UpdateClientSize(handle);
	LocalPlayerInstance->GetPlayerInstance()->UpdateTeam(handle);
	LocalPlayerInstance->GetPlayerInstance()->UpdateAlive(handle);
	LocalPlayerInstance->GetPlayerInstance()->UpdateStance(handle);
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	std::vector<std::shared_ptr<Player>> players;



	for (auto player : templayers)
	{
		if(!player->GetClientSize())
			continue;
		if (!player->GetAlive())
			continue;
		if (player->GetTeam() ==LocalPlayerInstance->GetPlayerInstance()->GetTeam())
			continue;
		players.push_back(player);

			
	
	}
	Players = players;
}
void Game::UpdatePlayers()
{
	if (PlayerCount == 0)
		return;

	auto handle = TargetProcess.CreateScatterHandle();
	LocalPlayerInstance->UpdateClientIndex(handle);
	LocalPlayerInstance->UpdateFov(handle);
	LocalPlayerInstance->UpdateViewAngles(handle);
	LocalPlayerInstance->UpdatePosition(handle);
	for (std::shared_ptr<Player> player : Players)
	{
		player->UpdateAlive(handle);
		player->UpdatePosition(handle);
		player->UpdateStance(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
}
std::vector<std::shared_ptr<Player>> Game::GetPlayers()
{
	return Players;
}
std::shared_ptr<LocalPlayer> Game::GetLocalPlayer()
{
	return LocalPlayerInstance;
}

