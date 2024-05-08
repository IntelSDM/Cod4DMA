#include "Pch.h"
#include "LocalPlayer.h"

void LocalPlayer::UpdateClientIndex(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, ClientIndexOffset, reinterpret_cast<void*>(&ClientIndex), sizeof(int));
}

void LocalPlayer::UpdateFov(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, FovOffset, reinterpret_cast<void*>(&Fov), sizeof(Vector2));
}

void LocalPlayer::UpdateViewAngles(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, ViewAnglesOffset, reinterpret_cast<void*>(&ViewAngles), sizeof(Vector3));
}

void LocalPlayer::UpdatePosition(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PositionOffeset, reinterpret_cast<void*>(&Position), sizeof(Vector3));
}

int LocalPlayer::GetClientIndex()
{
	return ClientIndex;
}

Vector2 LocalPlayer::GetFov()
{
	return Fov;
}

Vector3 LocalPlayer::GetViewAngles()
{
	return ViewAngles;
}

Vector3 LocalPlayer::GetPosition()
{
	return Position;
}

void LocalPlayer::SetUpPlayerInstance()
{
	PlayerInstance = std::make_shared<Player>(ClientIndex);
}