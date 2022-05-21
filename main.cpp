
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"




int main()
{
	DWORD procId = GetProcId(L"t6zm - Zombies Offline.exe");
	std::cout << "Process ID of \"t6zm - Zombies Offline.exe\" -> " << procId << std::endl;

	uintptr_t modBaseAddr = GetModuleBaseAddress(procId, L"t6zm - Zombies Offline.exe");	
	std::cout << "Module Base address -> " << "0x400000" << std::endl << std::endl;

	// Get Handle to process
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	// Resolving base addresses
	uintptr_t dynamicPtrBaseAddr = 0x400000 + 0x01D88290;
	std::cout << "Dynamic Pointer Base address: " << "0x400000" << " + 0x01D88290 = " << std::dec << "0x" << std::hex << dynamicPtrBaseAddr << std::endl << std::endl;
	
	// Resolve values
	uintptr_t entityAddr = 0x0;
	ReadProcessMemory(hProcess, (BYTE*)dynamicPtrBaseAddr, &entityAddr, sizeof(entityAddr), nullptr);
	std::cout << "Player entity obj addr -> " << entityAddr << std::endl;
	
	std::vector<unsigned int> ammoOffset = { 0x42C };
	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffset);

	std::cout << "Ammo address = " << "0x" << std::hex << ammoAddr << std::endl;

	// Read values
	int ammoValue = 0;

	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
	std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;

	// Writing to memory
	int newAmmo = 420;
	WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

	// Reading out
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

	std::cout << "New ammo = " << std::dec << ammoValue << std::endl;

	getchar();
	return 0;
}