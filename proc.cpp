#include "stdafx.h"
#include "proc.h"


DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;

	// Get Process snapshot
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		// Struct that contains process information
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(hSnap, &procEntry))
		{
			std::cout << "Failed to retrieve information about the first process." << std::endl;
			return procId;
		}

		while (Process32Next(hSnap, &procEntry))
		{
			std::wcout << "\nProcess " << procEntry.th32ProcessID << ": " << procEntry.szExeFile;

			if (!_wcsicmp(procEntry.szExeFile, procName))
			{
				std::cout << " <- Process Found!" << std::endl;
				procId = procEntry.th32ProcessID;
				break;
			}
		}
	}
	Sleep(500);
	CloseHandle(hSnap);
	return procId;
}

DWORD_PTR GetModuleBaseAddress64(DWORD processID)
{
	DWORD_PTR   baseAddress = 0;
	HANDLE      hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	HMODULE*	moduleArray;
	LPBYTE      moduleArrayBytes;
	DWORD       bytesRequired;

	if (hProc)
	{
		if (EnumProcessModules(hProc, NULL, 0, &bytesRequired))
		{
			if (bytesRequired)
			{
				moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

				if (moduleArrayBytes)
				{
					unsigned int moduleCount;

					moduleCount = bytesRequired / sizeof(HMODULE);
					moduleArray = (HMODULE*)moduleArrayBytes;

					if (EnumProcessModules(hProc, moduleArray, bytesRequired, &bytesRequired))
					{
						baseAddress = (DWORD_PTR)moduleArray[0];
					}

					LocalFree(moduleArrayBytes);
				}
			}
		}

		CloseHandle(hProc);
	}

	return baseAddress;
}

// Get the dynamic address given the offsets
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

void WriteToMemory(
	HANDLE hProcess, 
	uintptr_t playerPtrBase, 
	bool bHealth,
	bool bAmmo, 
	uintptr_t healthAddr,
	uintptr_t primaryAmmoAddr, 
	uintptr_t secondaryAmmoAddr	
	)
	{

	if (bAmmo)
	{
		//Write to memory ammo
		int newAmmo = 420;
		WriteProcessMemory(hProcess, (BYTE*)primaryAmmoAddr, &newAmmo, sizeof(newAmmo), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)secondaryAmmoAddr, &newAmmo, sizeof(newAmmo), nullptr);
	}
	if (bHealth)
	{
		//Write to memory health
		int newHealth = 420;
		WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);

	}
}