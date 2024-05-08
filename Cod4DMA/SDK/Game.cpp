#include "Pch.h"
#include "Game.h"

Game::Game()
{
	PlayerCount = TargetProcess.Read<short int>( PlayerCountOffset);
	for (int i = 0; i < PlayerCount; i++)
	{
		std::shared_ptr<Player> player = std::make_shared<Player>(i);
		auto handle = TargetProcess.CreateScatterHandle();
		player->UpdateClientSize(handle);
		player->UpdateTeam(handle);
		player->UpdateAlive(handle);
		player->UpdatePosition(handle);
		player->UpdateName(handle);
		player->UpdateStance(handle);
		TargetProcess.ExecuteReadScatter(handle);
		TargetProcess.CloseScatterHandle(handle);

		printf("Player %d\n", i);
		printf("ClientSize: %d\n", player->GetClientSize());
		printf("Team: %d\n", player->GetTeam());
		printf("Alive: %d\n", player->GetAlive());
		printf("Position: %f %f %f\n", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
		printf("Name: %s\n", player->GetName().c_str());
		printf("Stance: %d\n", player->GetStance());




	}
}

short int Game::GetPlayerCount()
{
	return PlayerCount;
}

void Game::UpdatePlayerCount(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PlayerCountOffset,reinterpret_cast<void*>(&PlayerCount), sizeof(short int));
}
