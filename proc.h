#pragma once

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

DWORD_PTR GetModuleBaseAddress64(DWORD processID);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

void WriteToMemory(
	HANDLE hProcess,
	uintptr_t playerPtrBase,
	bool bHealth,
	bool bAmmo,
	uintptr_t healthAddr,
	uintptr_t primaryAmmoAddr,
	uintptr_t secondaryAmmoAddr
);