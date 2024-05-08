#pragma once
#include "player.h"

class LocalPlayer
{
private:
	uint32_t ClientIndexOffset = 0x74E338;
	int ClientIndex = 0;
	uint32_t FovOffset = 0x797610;
	Vector2 Fov = { 0, 0 };
	uint32_t ViewAnglesOffset = 0x79B698;
	Vector3 ViewAngles = { 0, 0, 0 };
	uint32_t PositionOffeset = 0x797618;
	Vector3 Position = { 0, 0, 0 };

	std::shared_ptr<Player> PlayerInstance;

public:
	void UpdateClientIndex(VMMDLL_SCATTER_HANDLE handle);
	void UpdateFov(VMMDLL_SCATTER_HANDLE handle);
	void UpdateViewAngles(VMMDLL_SCATTER_HANDLE handle);
	void UpdatePosition(VMMDLL_SCATTER_HANDLE handle);

	int GetClientIndex();
	Vector2 GetFov();
	Vector3 GetViewAngles();
	Vector3 GetPosition();

	void SetUpPlayerInstance();

	Vector2 WorldToScreen(Vector3 world);
	std::shared_ptr<Player> GetPlayerInstance();
};