#include "Pch.h"
#include "LocalPlayer.h"
#include "ConfigUtilities.h"

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


Vector2 LocalPlayer::WorldToScreen(Vector3 world)
{
	Vector3 vLocal, vTransForm;
	Vector3 forward = Vector3();
	Vector3 right = Vector3();
	Vector3 upward = Vector3();

	float angle;
	float sr, sp, sy, cr, cp, cy,
		cpi = (3.141f * 2 / 360);


	//cpi = same view angles.x isn't
	angle = ViewAngles.x * cpi;
	sp = (float)sin(angle);
	cp = (float)cos(angle);
	angle = ViewAngles.y * cpi;
	sy = (float)sin(angle);
	cy = (float)cos(angle);
	angle = ViewAngles.z * cpi;
	sr = (float)sin(angle);
	cr = (float)cos(angle);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;


	right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	right.y = (-1 * sr * sp * sy + -1 * cr * cy);
	right.z = -1 * sr * cp;


	upward.x = (-cr * sp * cy + -sr * -sy);
	upward.y = (-cr * sp * sy + -sr * cy);
	upward.z = -cr * cp;
	vLocal = Vector3(world.x - Position.x, world.y - Position.y, world.z - Position.z);

	vTransForm.x = Vector3::Dot(vLocal,right);
	vTransForm.y = Vector3::Dot(vLocal, upward);
	vTransForm.z = Vector3::Dot(vLocal, forward);


	if (vTransForm.z < 0.01)
		return Vector2::Zero();
	Vector2 screen = Vector2::Zero();
	Vector2 centre = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height / 2 : GetSystemMetrics(SM_CYSCREEN) / 2);
	screen.x = centre.x+ (centre.x / vTransForm.z * (1 / Fov.x)) * vTransForm.x;
	screen.y = centre.y + (centre.y / vTransForm.z * (1 / Fov.y)) * vTransForm.y;
	return screen;
}

std::shared_ptr<Player> LocalPlayer::GetPlayerInstance()
{
	return PlayerInstance;
}