#pragma once
#include "LocalPlayer.h"
#include "Player.h"
class Game
{
private:
	uint32_t PlayerCountOffset = 0x774350;
	short int PlayerCount = 0;
	std::vector<std::shared_ptr<Player>> Players;
	std::shared_ptr<LocalPlayer> LocalPlayerInstance;
public: 
	Game();
	short int GetPlayerCount();
	void UpdatePlayerCount(VMMDLL_SCATTER_HANDLE handle);
	void Cache();
	void UpdatePlayers();
	std::vector<std::shared_ptr<Player>> GetPlayers();
	std::shared_ptr<LocalPlayer> GetLocalPlayer();
};