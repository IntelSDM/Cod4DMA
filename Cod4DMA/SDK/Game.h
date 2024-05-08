#pragma once
#include "LocalPlayer.h"
#include "Player.h"
class Game
{
private:
	uint32_t PlayerCountOffset = 0x774350;
	short int PlayerCount = 0;
public: 
	Game();
	short int GetPlayerCount();
	void UpdatePlayerCount(VMMDLL_SCATTER_HANDLE handle);
};