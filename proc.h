#pragma once

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <iostream>
#include "MemMan.h"
#include "Addr.h"
#include "Values.h"

DWORD GetProcId(const wchar_t* procName);

DWORD_PTR GetModuleBaseAddress64(DWORD processID);

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

int GetProcInfo();

// void WriteToMemory(HANDLE hProcess,	uintptr_t moduleBaseAddr, uintptr_t playerPtrBase, bool bHealth, bool bAmmo, bool bFireRate, bool bRecoil, PlayerAddr *_pAddrPtr);