#include "Pch.h"
#include "Player.h"

void Player::UpdateClientSize(VMMDLL_SCATTER_HANDLE handle)
{
		TargetProcess.AddScatterReadRequest(handle, ClientInfoOffset +Iteration *ClientSizeOffset, reinterpret_cast<void*>(&ClientSize), sizeof(int));
}

void Player::UpdateEntityIndex(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PlayerInfoOffset + Iteration * EntitySize + EntityIndexOffset, reinterpret_cast<void*>(&EntityIndex), sizeof(int));
}

void Player::UpdateTeam(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, ClientInfoOffset + Iteration * ClientSizeOffset + TeamOffset, reinterpret_cast<void*>(&Team), sizeof(int));
}

void Player::UpdateAlive(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PlayerInfoOffset + Iteration * EntitySize + AliveOffset, reinterpret_cast<void*>(&Alive), sizeof(bool));
}

void Player::UpdatePosition(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PlayerInfoOffset + Iteration * EntitySize + PositionOffset, reinterpret_cast<void*>(&Position), sizeof(Vector3));
}

void Player::UpdateStance(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, ClientInfoOffset + Iteration * ClientSizeOffset + StanceOffset, reinterpret_cast<void*>(&Stance), sizeof(int));
}


int Player::GetIteration()
{
	return Iteration;
}

int Player::GetClientSize()
{
	return ClientSize;
}

int Player::GetEntityIndex()
{
	return EntityIndex;
}

int Player::GetTeam()
{
	return Team;
}

bool Player::GetAlive()
{
	return Alive;
}

Vector3 Player::GetPosition()
{
	return Position;
}

int Player::GetStance()
{
	return Stance;
}

Player::Player(int itterator)
{
	Iteration = itterator;
}

Stances Player::GetConveredStance()
{
	if (Stance == 2 || Stance == 1024 || Stance == 2048 || Stance == 1048576 || Stance == 0 || Stance == 16 || Stance == 32)	// 1048576 is sprinting		0 is broken ankles		There's a lot of different values for mounting objects
		return Stances::Standing;
	else if (Stance == 4 || Stance == 4096 || Stance == 8192 || Stance == 64 || Stance == 128)	// last two are moving while aiming
		return Stances::Crouching;
	else if (Stance == 8 || Stance == 256 || Stance == 512)
		return Stances::Prone;
	else
	return Stances::Other;

}

Vector3 Player::GetHeadWorldPosition()
{
	Stances stance = GetConveredStance();
if(stance == Stances::Standing)
	return Vector3(Position.x, Position.y, Position.z + 60);
else if(stance == Stances::Crouching)
	return Vector3(Position.x, Position.y, Position.z + 40);
else if(stance == Stances::Prone)
	return Vector3(Position.x, Position.y, Position.z + 21);
else if (stance == Stances::LastStand)
return Vector3(Position.x, Position.y, Position.z + 21);
else
	return Vector3(Position.x, Position.y, Position.z + 50);
}