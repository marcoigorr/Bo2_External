
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{
	HANDLE hProcess = 0;
	uintptr_t moduleBaseAddr = 0, localPlayerPtr = 0, localPlayerPtrSuper = 0;
	uintptr_t nameAddr = 0, healthAddr = 0, healthMaxAddr = 0, pointsAddr = 0, primaryAmmoAddr = 0, primaryMagAddr = 0, secondaryAmmoAddr = 0, secondaryMagAddr = 0, grenadesAddr = 0;
	std::string nameValue = "Game Name Here";
	int healthValue = 0, healthMaxValue = 0, pointsValue = 0, primaryAmmoValue = 0, primaryMagValue = 0, secondaryAmmoValue = 0, secondaryMagValue = 0, grenadesValue = 0;

	bool bHealth = false, bAmmo = false;

	// For console output
	bool UpdateOnNextRun = true;
	int timeSinceLastUpdate = clock();
	std::string sAmmoStatus = "OFF";
	std::string sHealthStatus = "OFF";

	// Offsets
	std::vector<unsigned int> nameOffset = { 0x5534 };
	std::vector<unsigned int> healthOffset = { 0x1A8 };
	std::vector<unsigned int> healthMaxOffset = { 0x1AC };
	std::vector<unsigned int> pointsOffset = { 0x55C8 };
	std::vector<unsigned int> primaryAmmoOffset = { 0x42C };
	std::vector<unsigned int> primaryMagOffset = { 0x3F0 };
	std::vector<unsigned int> secondaryAmmoOffset = { 0x434 };
	std::vector<unsigned int> secondaryMagOffset = { 0x3F8 };
	std::vector<unsigned int> grenadesOffset = { 0x430 };

	DWORD procId = GetProcId(L"t6zm - Zombies Offline.exe");

	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

		moduleBaseAddr = GetModuleBaseAddress64(procId);

		localPlayerPtr = moduleBaseAddr + 0x01D88290;
		localPlayerPtrSuper = moduleBaseAddr + 0x01F38784;

		nameAddr = FindDMAAddy(hProcess, localPlayerPtr, nameOffset);
		healthAddr = FindDMAAddy(hProcess, localPlayerPtrSuper, healthOffset);
		healthMaxAddr = FindDMAAddy(hProcess, localPlayerPtrSuper, healthMaxOffset);
		pointsAddr = FindDMAAddy(hProcess, localPlayerPtr, pointsOffset);
		primaryAmmoAddr = FindDMAAddy(hProcess, localPlayerPtr, primaryAmmoOffset);
		primaryMagAddr = FindDMAAddy(hProcess, localPlayerPtr, primaryMagOffset);
		secondaryAmmoAddr = FindDMAAddy(hProcess, localPlayerPtr, secondaryAmmoOffset);
		secondaryMagAddr = FindDMAAddy(hProcess, localPlayerPtr, secondaryMagOffset);
		grenadesAddr = FindDMAAddy(hProcess, localPlayerPtr, grenadesOffset);
	}
	else
	{
		std::cout << "Process not found... press any key to exit.  ";
		getchar();
		return 0;
	}

	DWORD dwExitCode = 0;

	while (GetExitCodeProcess(hProcess, &dwExitCode) && dwExitCode == STILL_ACTIVE)
	{	
		if (UpdateOnNextRun || clock() - timeSinceLastUpdate > 1000)
		{
			system("cls");

			// Health
			ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), NULL);
			ReadProcessMemory(hProcess, (BYTE*)healthMaxAddr, &healthMaxValue, sizeof(healthMaxValue), NULL);
			// Points
			ReadProcessMemory(hProcess, (BYTE*)pointsAddr, &pointsValue, sizeof(pointsValue), NULL);
			// Primary Weapon
			ReadProcessMemory(hProcess, (BYTE*)primaryAmmoAddr, &primaryAmmoValue, sizeof(primaryAmmoValue), NULL);
			ReadProcessMemory(hProcess, (BYTE*)primaryMagAddr, &primaryMagValue, sizeof(primaryMagValue), NULL);
			// Secondary Weapon
			ReadProcessMemory(hProcess, (BYTE*)secondaryAmmoAddr, &secondaryAmmoValue, sizeof(secondaryAmmoValue), NULL);
			ReadProcessMemory(hProcess, (BYTE*)secondaryMagAddr, &secondaryMagValue, sizeof(secondaryMagValue), NULL);

			ReadProcessMemory(hProcess, (BYTE*)grenadesAddr, &grenadesValue, sizeof(grenadesValue), NULL);

			std::cout << "----------------------------------------------------------------------" << std::endl;
			std::cout << "                     t6zm - Zombies Offline Cheats" << std::endl;
			std::cout << "----------------------------------------------------------------------" << std::endl << std::endl;
			std::cout << "\t[F1] God Mode -> " << sHealthStatus << std::endl;
			std::cout << "\t[F2] Unlimited Ammo -> " << sAmmoStatus << std::endl;
			std::cout << "\t[F3] Add 500 points " << std::endl;
			std::cout << "\n--------------------------------- INFO -----------------------------\n" << std::endl;

			std::cout << "  Health         -> " << "0x" << std::hex << healthAddr << "\tValue = " << std::dec << healthValue << std::endl;
			std::cout << "  Health Max     -> " << "0x" << std::hex << healthMaxAddr << "\tValue = " << std::dec << healthMaxValue << std::endl;

			std::cout << "  Points         -> " << "0x" << std::hex << pointsAddr << "\tValue = " << std::dec << pointsValue << std::endl;

			std::cout << "  Primary Ammo   -> " << "0x" << std::hex << primaryAmmoAddr << "\tValue = " << std::dec << primaryAmmoValue << std::endl;
			std::cout << "  Primary Mag    -> " << "0x" << std::hex << primaryMagAddr << "\tValue = " << std::dec << primaryMagValue << std::endl;

			std::cout << "  Secondary Ammo -> " << "0x" << std::hex << secondaryAmmoAddr << "\tValue = " << std::dec << secondaryAmmoValue << std::endl;
			std::cout << "  Secondary Mag  -> " << "0x" << std::hex << secondaryMagAddr << "\tValue = " << std::dec << secondaryMagValue << std::endl;

			std::cout << "  Grenades       -> " << "0x" << std::hex << grenadesAddr << "\tValue = " << std::dec << grenadesValue << std::endl;

			UpdateOnNextRun = false;
			timeSinceLastUpdate = clock();
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

		// Primary Ammo
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

		// Points
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			UpdateOnNextRun = true;
			pointsValue += 500;
			WriteProcessMemory(hProcess, (BYTE*)pointsAddr, &pointsValue, sizeof(pointsValue), NULL);
		}

		WriteToMemory(hProcess, localPlayerPtr, bHealth, bAmmo, healthAddr, primaryAmmoAddr, secondaryAmmoAddr);
	}


	std::cout << "Process not found... Press any key to exit.  ";
	getchar();
	return 0;
}
