
#include "stdafx.h"

void findAddr(HANDLE hProcess, uintptr_t localPlayerPtr, uintptr_t entList, PlayerAddr* _pAddr, PlayerOffsets* _pOffsets);
void readValues(HANDLE hProcess, PlayerAddr* _pAddr, PlayerValues* _pValues);

int main()
{
	PlayerAddr _pAddr;
	PlayerValues _pValues;
	PlayerOffsets _pOffsets;

	HANDLE hProcess = 0;
	uintptr_t moduleBaseAddr, localPlayerPtr, entList, cg;

	bool bHealth = false, bAmmo = false, bFireRate = false, bRecoil = false;

	// For console output
	bool UpdateOnNextRun = true;
	int timeSinceLastUpdate = clock();
	std::string sAmmoStatus = "OFF";
	std::string sHealthStatus = "OFF";
	std::string sFireRateStatus = "OFF";
	std::string sRecoilStatus = "OFF";

	// Get process ID
	DWORD procId = GetProcId(L"t6zm - Zombies Offline.exe");

	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

		moduleBaseAddr = GetModuleBaseAddress64(procId);

		localPlayerPtr = moduleBaseAddr + 0x01D88290;
		entList = moduleBaseAddr + 0x01F387A8;
		cg = moduleBaseAddr + 0x0103AC50; // client game

		findAddr(hProcess, localPlayerPtr, entList, &_pAddr, &_pOffsets);
	}
	else
	{
		std::cout << "\n\nProcess not found... press any key to exit.  ";
		getchar();
		return 0;
	}

	DWORD dwExitCode = 0;

	while (GetExitCodeProcess(hProcess, &dwExitCode) && dwExitCode == STILL_ACTIVE)
	{	
		if (UpdateOnNextRun || clock() - timeSinceLastUpdate > 2000)
		{
			system("cls");

			readValues(hProcess, &_pAddr, &_pValues);

			std::cout << "----------------------------------------------------------------------" << std::endl;
			std::cout << "                     t6zm - Zombies Offline Cheats" << std::endl;
			std::cout << "----------------------------------------------------------------------" << std::endl << std::endl;
			std::cout << "\t[F1] God Mode -> " << sHealthStatus << std::endl;
			std::cout << "\t[F2] Unlimited Ammo -> " << sAmmoStatus << std::endl;
			std::cout << "\t[F3] Fire Rate Hack -> " << sFireRateStatus << std::endl;
			std::cout << "\t[F4] Not so Recoil  -> " << sRecoilStatus << std::endl;
			std::cout << "\t[F5] Add 500 points " << std::endl;
			std::cout << "\t[F6] Add a Grenade " << std::endl;

			std::cout << "\n\t[F10] Update Addresses " << std::endl;
			std::cout << "\n--------------------------------- INFO -----------------------------\n" << std::endl;

			std::cout << "  Module Base Addr      -> " << "0x" << std::hex << moduleBaseAddr << std::endl;
			std::cout << "  Local player pointer  -> " << "0x" << std::hex << localPlayerPtr << std::endl;
			std::cout << "  Entity List           -> " << "0x" << std::hex << entList << std::endl << std::endl;
			std::cout << "  Health         -> " << "0x" << std::hex << _pAddr.health << "\tValue = " << std::dec << _pValues.health << std::endl;
			std::cout << "  Health Max     -> " << "0x" << std::hex << _pAddr.healthMax << "\tValue = " << std::dec << _pValues.healthMax << std::endl;
			std::cout << "  Points         -> " << "0x" << std::hex << _pAddr.points << "\tValue = " << std::dec << _pValues.points << std::endl;
			std::cout << "  Primary Mag    -> " << "0x" << std::hex << _pAddr.primaryMag << "\tValue = " << std::dec << _pValues.primaryMag << std::endl;
			std::cout << "  Secondary Mag  -> " << "0x" << std::hex << _pAddr.secondaryMag << "\tValue = " << std::dec << _pValues.secondaryMag << std::endl;
			std::cout << "  Ammo 1         -> " << "0x" << std::hex << _pAddr.ammo1 << "\tValue = " << std::dec << _pValues.ammo1 << std::endl;
			std::cout << "  Ammo 2         -> " << "0x" << std::hex << _pAddr.ammo2 << "\tValue = " << std::dec << _pValues.ammo2 << std::endl;
			std::cout << "  Ammo 3         -> " << "0x" << std::hex << _pAddr.ammo3 << "\tValue = " << std::dec << _pValues.ammo3 << std::endl;
			std::cout << "  Ammo 4         -> " << "0x" << std::hex << _pAddr.ammo4 << "\tValue = " << std::dec << _pValues.ammo4 << std::endl;
			std::cout << "  Ammo 5         -> " << "0x" << std::hex << _pAddr.ammo5 << "\tValue = " << std::dec << _pValues.ammo5 << std::endl;
			std::cout << "  Grenades       -> " << "0x" << std::hex << _pAddr.grenades << "\tValue = " << std::dec << _pValues.grenades << std::endl;

			UpdateOnNextRun = false;
			timeSinceLastUpdate = clock();
		}

		// Recalculate addresses
		if (GetAsyncKeyState(VK_F10) & 1)
		{
			UpdateOnNextRun = true;

			findAddr(hProcess, localPlayerPtr, entList, &_pAddr, &_pOffsets);
		}

		// Health
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			UpdateOnNextRun = true;

			bHealth = !bHealth;
			if (bHealth)
			{
				sHealthStatus = "ON";
			}
			else
			{
				sHealthStatus = "OFF";
			}
		}

		// Ammo
		if (GetAsyncKeyState(VK_F2) & 1)
		{
			UpdateOnNextRun = true;

			bAmmo = !bAmmo;
			if (bAmmo)
			{
				sAmmoStatus = "ON";
			}
			else
			{
				sAmmoStatus = "OFF";
			}
		}

		// Fire Rate
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			UpdateOnNextRun = true;

			bFireRate = !bFireRate;
			if (bFireRate)
			{
				sFireRateStatus = "ON";
			}
			else
			{
				sFireRateStatus = "OFF";
			}

		}

		// Recoil
		if (GetAsyncKeyState(VK_F4) & 1)
		{
			UpdateOnNextRun = true;

			bRecoil = !bRecoil;
			if (bRecoil)
			{
				sRecoilStatus = "ON";
			}
			else
			{
				sRecoilStatus = "OFF";
			}
		}

		// Points
		if (GetAsyncKeyState(VK_F5) & 1)
		{
			UpdateOnNextRun = true;
			_pValues.points += 500;
			WriteProcessMemory(hProcess, (BYTE*)_pAddr.points, &_pValues.points, sizeof(_pValues.points), NULL);
		}

		// Primary Grenade +1
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			UpdateOnNextRun = true;
			_pValues.grenades += 1;
			WriteProcessMemory(hProcess, (BYTE*)_pAddr.grenades, &_pValues.grenades, sizeof(_pValues.grenades), NULL);
		}

		WriteToMemory(hProcess, moduleBaseAddr, localPlayerPtr, bHealth, bAmmo, bFireRate, bRecoil, &_pAddr);
	}

	system("cls");
	std::cout << "Process closed or not found... Press any key to exit.  ";
	getchar();
	return 0;
}

void findAddr(HANDLE hProcess, uintptr_t localPlayerPtr, uintptr_t entList, PlayerAddr *_pAddr, PlayerOffsets *_pOffsets)
{
	// Very nice code right here
	_pAddr->name = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->name);
	_pAddr->health = FindDMAAddy(hProcess, entList, _pOffsets->health);
	_pAddr->healthMax = FindDMAAddy(hProcess, entList, _pOffsets->healthMax);
	_pAddr->points = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->points);
	_pAddr->primaryMag = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->primaryMag);
	_pAddr->secondaryMag = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->secondaryMag);
	_pAddr->ammo1 = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->ammo1);
	_pAddr->ammo2 = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->ammo2);
	_pAddr->ammo3 = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->ammo3);
	_pAddr->ammo4 = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->ammo4);
	_pAddr->ammo5 = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->ammo5);
	_pAddr->grenades = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->grenades);
	_pAddr->crossHair = FindDMAAddy(hProcess, localPlayerPtr, _pOffsets->crossHair);
}

void readValues(HANDLE hProcess, PlayerAddr *_pAddr, PlayerValues *_pValues)
{
	// Very nice code right here 2
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->health, &_pValues->health, sizeof(_pValues->health), NULL);
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->healthMax, &_pValues->healthMax, sizeof(_pValues->healthMax), NULL);
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->points, &_pValues->points, sizeof(_pValues->points), NULL);
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->primaryMag, &_pValues->primaryMag, sizeof(_pValues->primaryMag), NULL);
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->secondaryMag, &_pValues->secondaryMag, sizeof(_pValues->secondaryMag), NULL);
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->ammo1, &_pValues->ammo1, sizeof(_pValues->ammo1), NULL);
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->ammo2, &_pValues->ammo2, sizeof(_pValues->ammo2), NULL);
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->ammo3, &_pValues->ammo3, sizeof(_pValues->ammo3), NULL);
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->ammo4, &_pValues->ammo4, sizeof(_pValues->ammo4), NULL);
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->ammo5, &_pValues->ammo5, sizeof(_pValues->ammo5), NULL);
	ReadProcessMemory(hProcess, (BYTE*)_pAddr->grenades, &_pValues->grenades, sizeof(_pValues->grenades), NULL);
}