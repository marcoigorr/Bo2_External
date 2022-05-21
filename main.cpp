
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"


int main()
{
	/* ------------------- Process ------------------- */
	DWORD procId = GetProcId(L"t6zm - Zombies Offline.exe");

	DWORD_PTR modBaseAddr = GetModuleBaseAddress64(procId);

	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	uintptr_t dynamicPtrBaseAddr = modBaseAddr + 0x01D88290;

	// Process Info
	std::cout << "Process ID of \"t6zm - Zombies Offline.exe\" -> " << std::dec << procId << std::endl;
	std::cout << "Module Base addr -> " << "0x" << std::hex << modBaseAddr << std::endl;
	std::cout << "Dynamic Pointer Base addr -> " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl << std::endl;

	/* ------------------- Entity ------------------- */

	// Resolving Entity Object Base addr
	uintptr_t playerAddr = 0x0;
	ReadProcessMemory(hProcess, (BYTE*)dynamicPtrBaseAddr, &playerAddr, sizeof(playerAddr), NULL);
	std::cout << "Player Object addr = " << "0x" << std::hex << playerAddr << std::endl;

	// Primary Ammo
	std::vector<unsigned int> ammoOffsets = { 0x42C };
	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);
	std::cout << "Ammo addr = " << "0x" << std::hex << ammoAddr << std::endl;

	int ammoValue = 0;
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), NULL);
	std::cout << "Ammo value = " << std::dec << ammoValue << std::endl;

	int newAmmoValue = 420;
	WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmoValue, sizeof(newAmmoValue), NULL);

	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), NULL);
	std::cout << "New ammo = " << std::dec << ammoValue << std::endl;

	// Wait 
	int x;
	std::cin >> x;
	return 0;
}