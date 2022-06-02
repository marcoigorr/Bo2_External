
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

void updateAddr(DWORD procId)
{
	
}

int main()
{
	HANDLE hProcess = 0;
	uintptr_t moduleBaseAddr, localPlayerPtr, localPlayerPtrSuper;
	uintptr_t nameAddr, healthAddr, healthMaxAddr, pointsAddr, primaryMagAddr, secondaryMagAddr, ammo1Addr, ammo2Addr, ammo3Addr, ammo4Addr, ammo5Addr, grenadesAddr;
	uintptr_t crossHairAddr;
	std::string nameValue = "Game Name Here";
	int healthValue, healthMaxValue, pointsValue, primaryMagValue, secondaryMagValue, ammo1Value, ammo2Value, ammo3Value, ammo4Value, ammo5Value, grenadesValue;
	int crossHairValue;

	bool bHealth = false, bAmmo = false, bFireRate = false, bRecoil = false;

	// For console output
	bool UpdateOnNextRun = true;
	int timeSinceLastUpdate = clock();
	std::string sAmmoStatus = "OFF";
	std::string sHealthStatus = "OFF";
	std::string sFireRateStatus = "OFF";
	std::string sRecoilStatus = "OFF";

	// Offsets
	std::vector<unsigned int> nameOffset = { 0x5534 };
	std::vector<unsigned int> healthOffset = { 0x1A8 };
	std::vector<unsigned int> healthMaxOffset = { 0x1AC };
	std::vector<unsigned int> pointsOffset = { 0x55C8 };

	std::vector<unsigned int> primaryMagOffset = { 0x3F0 };
	std::vector<unsigned int> secondaryMagOffset = { 0x3F8 };
	std::vector<unsigned int> ammo1Offset = { 0x42C };
	std::vector<unsigned int> ammo2Offset = { 0x43C };
	std::vector<unsigned int> ammo3Offset = { 0x434 };
	std::vector<unsigned int> ammo4Offset = { 0x440 };
	std::vector<unsigned int> ammo5Offset = { 0x438 };

	std::vector<unsigned int> grenadesOffset = { 0x430 };
	
	std::vector<unsigned int> crossHairOffset = { 0x588 };

	// Get process ID
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
		primaryMagAddr = FindDMAAddy(hProcess, localPlayerPtr, primaryMagOffset);
		secondaryMagAddr = FindDMAAddy(hProcess, localPlayerPtr, secondaryMagOffset);
		ammo1Addr = FindDMAAddy(hProcess, localPlayerPtr, ammo1Offset);
		ammo2Addr = FindDMAAddy(hProcess, localPlayerPtr, ammo2Offset);
		ammo3Addr = FindDMAAddy(hProcess, localPlayerPtr, ammo3Offset);
		ammo4Addr = FindDMAAddy(hProcess, localPlayerPtr, ammo4Offset);
		ammo5Addr = FindDMAAddy(hProcess, localPlayerPtr, ammo5Offset);
		grenadesAddr = FindDMAAddy(hProcess, localPlayerPtr, grenadesOffset);
		crossHairAddr = FindDMAAddy(hProcess, localPlayerPtr, crossHairOffset);

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
		if (UpdateOnNextRun || clock() - timeSinceLastUpdate > 1000)
		{
			system("cls");

			// Health
			ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), NULL);
			ReadProcessMemory(hProcess, (BYTE*)healthMaxAddr, &healthMaxValue, sizeof(healthMaxValue), NULL);
			// Points
			ReadProcessMemory(hProcess, (BYTE*)pointsAddr, &pointsValue, sizeof(pointsValue), NULL);
			// Weapons
			ReadProcessMemory(hProcess, (BYTE*)primaryMagAddr, &primaryMagValue, sizeof(primaryMagValue), NULL);
			ReadProcessMemory(hProcess, (BYTE*)secondaryMagAddr, &secondaryMagValue, sizeof(secondaryMagValue), NULL);
			ReadProcessMemory(hProcess, (BYTE*)ammo1Addr, &ammo1Value, sizeof(ammo1Value), NULL);
			ReadProcessMemory(hProcess, (BYTE*)ammo2Addr, &ammo2Value, sizeof(ammo2Value), NULL);
			ReadProcessMemory(hProcess, (BYTE*)ammo3Addr, &ammo3Value, sizeof(ammo3Value), NULL);
			ReadProcessMemory(hProcess, (BYTE*)ammo4Addr, &ammo4Value, sizeof(ammo4Value), NULL);
			ReadProcessMemory(hProcess, (BYTE*)ammo5Addr, &ammo5Value, sizeof(ammo5Value), NULL);

			// Grenades
			ReadProcessMemory(hProcess, (BYTE*)grenadesAddr, &grenadesValue, sizeof(grenadesValue), NULL);

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

			std::cout << "  Module Base Addr           -> " << "0x" << std::hex << moduleBaseAddr << std::endl;
			std::cout << "  Local player pointer       -> " << "0x" << std::hex << localPlayerPtr << std::endl;
			std::cout << "  Local player pointer super -> " << "0x" << std::hex << localPlayerPtrSuper << std::endl << std::endl;

			std::cout << "  Health         -> " << "0x" << std::hex << healthAddr << "\tValue = " << std::dec << healthValue << std::endl;
			std::cout << "  Health Max     -> " << "0x" << std::hex << healthMaxAddr << "\tValue = " << std::dec << healthMaxValue << std::endl;
			std::cout << "  Points         -> " << "0x" << std::hex << pointsAddr << "\tValue = " << std::dec << pointsValue << std::endl;
			std::cout << "  Primary Mag    -> " << "0x" << std::hex << primaryMagAddr << "\tValue = " << std::dec << primaryMagValue << std::endl;
			std::cout << "  Secondary Mag  -> " << "0x" << std::hex << secondaryMagAddr << "\tValue = " << std::dec << secondaryMagValue << std::endl;
			std::cout << "  Ammo 1         -> " << "0x" << std::hex << ammo1Addr << "\tValue = " << std::dec << ammo1Value << std::endl;
			std::cout << "  Ammo 2         -> " << "0x" << std::hex << ammo2Addr << "\tValue = " << std::dec << ammo2Value << std::endl;
			std::cout << "  Ammo 3         -> " << "0x" << std::hex << ammo3Addr << "\tValue = " << std::dec << ammo3Value << std::endl;
			std::cout << "  Ammo 4         -> " << "0x" << std::hex << ammo4Addr << "\tValue = " << std::dec << ammo4Value << std::endl;
			std::cout << "  Ammo 5         -> " << "0x" << std::hex << ammo5Addr << "\tValue = " << std::dec << ammo5Value << std::endl;

			std::cout << "  Grenades       -> " << "0x" << std::hex << grenadesAddr << "\tValue = " << std::dec << grenadesValue << std::endl;

			UpdateOnNextRun = false;
			timeSinceLastUpdate = clock();
		}

		if (GetAsyncKeyState(VK_F10) & 1)
		{
			UpdateOnNextRun = true;

			nameAddr = FindDMAAddy(hProcess, localPlayerPtr, nameOffset);
			healthAddr = FindDMAAddy(hProcess, localPlayerPtrSuper, healthOffset);
			healthMaxAddr = FindDMAAddy(hProcess, localPlayerPtrSuper, healthMaxOffset);
			pointsAddr = FindDMAAddy(hProcess, localPlayerPtr, pointsOffset);
			primaryMagAddr = FindDMAAddy(hProcess, localPlayerPtr, primaryMagOffset);
			secondaryMagAddr = FindDMAAddy(hProcess, localPlayerPtr, secondaryMagOffset);
			ammo1Addr = FindDMAAddy(hProcess, localPlayerPtr, ammo1Offset);
			ammo2Addr = FindDMAAddy(hProcess, localPlayerPtr, ammo2Offset);
			ammo3Addr = FindDMAAddy(hProcess, localPlayerPtr, ammo3Offset);
			ammo4Addr = FindDMAAddy(hProcess, localPlayerPtr, ammo4Offset);
			ammo5Addr = FindDMAAddy(hProcess, localPlayerPtr, ammo5Offset);
			grenadesAddr = FindDMAAddy(hProcess, localPlayerPtr, grenadesOffset);
			crossHairAddr = FindDMAAddy(hProcess, localPlayerPtr, crossHairOffset);
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
			pointsValue += 500;
			WriteProcessMemory(hProcess, (BYTE*)pointsAddr, &pointsValue, sizeof(pointsValue), NULL);
		}

		// Primary Grenade +1
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			UpdateOnNextRun = true;
			grenadesValue += 1;
			WriteProcessMemory(hProcess, (BYTE*)grenadesAddr, &grenadesValue, sizeof(grenadesValue), NULL);
		}

		WriteToMemory(hProcess, moduleBaseAddr, localPlayerPtr, bHealth, bAmmo, bFireRate, bRecoil, healthAddr, ammo1Addr, ammo2Addr, ammo3Addr, ammo4Addr, ammo5Addr, crossHairAddr);
	}

	system("cls");
	std::cout << "Process closed or not found... Press any key to exit.  ";
	getchar();
	return 0;
}
