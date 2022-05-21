#include "stdafx.h"
#include "proc.h"


// Process ID is a DWORD type var
DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = (CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)); // Get a handle to the snapshot of all the current processes running

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry; // The pointer to a processentry structure that contains process informations

		procEntry.dwSize = sizeof(procEntry); // Setting the size of the structure, in bytes. Process32First fails otherwise

		if (Process32First(hSnap, &procEntry)) // Returns TRUE if the first entry of the process list has been copied to the buffer or FALSE otherwise
		{
			while (Process32Next(hSnap, &procEntry)) // Returns TRUE if the next entry of the process list has been copied to the buffer or FALSE otherwise
			{
				if (!_wcsicmp(procEntry.szExeFile, procName)) // Returns 0 if procName is equal to the iterated process
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			}
		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry))
		{
			while (Module32Next(hSnap, &modEntry))
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			}
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}