#pragma once
/*
dwCEntities = 0x8472D8
dwCClients = 0x831270
dwLocalIndex = 0x746338
dwRefdef = 0x78F600
EntitySize = 0x1DC
ClientSize = 0x4CC
*/
class Player
{
private:
	int Iteration = 0;
	uint32_t ClientSizeOffset = 0x4CC;
	int ClientSize = 0;
	uint32_t EntitySize = 0x1DC;
	int EntityIndex = 0;
	uint32_t EntityIndexOffset = 0xCC;
	uint32_t PlayerInfoOffset = 0x84F2D8;
	uint32_t ClientInfoOffset = 0x839270;
	uint32_t TeamOffset = 0x20;
	int Team = 0;
	uint32_t AliveOffset = 0x1C0;
	bool Alive = false;
	uint32_t PositionOffset = 0x1C;
	Vector3 Position = { 0, 0, 0 };
	uint32_t StanceOffset = 0x470;
	int Stance = 0;
	uint32_t NameOffset = 0xc;
	char Name[16];

public:
	Player(int itterator);
	void UpdateClientSize(VMMDLL_SCATTER_HANDLE handle);
	void UpdateEntityIndex(VMMDLL_SCATTER_HANDLE handle);
	void UpdateTeam(VMMDLL_SCATTER_HANDLE handle);
	void UpdateAlive(VMMDLL_SCATTER_HANDLE handle);
	void UpdatePosition(VMMDLL_SCATTER_HANDLE handle);
	void UpdateStance(VMMDLL_SCATTER_HANDLE handle);
	void UpdateName(VMMDLL_SCATTER_HANDLE handle);

	int GetIteration();
	int GetClientSize();
	int GetEntityIndex();
	int GetTeam();
	bool GetAlive();
	Vector3 GetPosition();
	int GetStance();
	std::wstring GetName();

};