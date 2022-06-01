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
	uintptr_t moduleBaseAddr,
	uintptr_t playerPtrBase, 
	bool bHealth,
	bool bAmmo, 
	bool bFireRate,
	bool bRecoil,
	uintptr_t healthAddr,
	uintptr_t ammo1Addr, 
	uintptr_t ammo2Addr,
	uintptr_t ammo3Addr,
	uintptr_t ammo4Addr,
	uintptr_t ammo5Addr,
	uintptr_t crossHairAddr
	)
	{
	// Ammo
	if (bAmmo)
	{
		//Write to memory ammo
		int newAmmo = 420;
		WriteProcessMemory(hProcess, (BYTE*)ammo1Addr, &newAmmo, sizeof(newAmmo), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)ammo2Addr, &newAmmo, sizeof(newAmmo), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)ammo3Addr, &newAmmo, sizeof(newAmmo), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)ammo4Addr, &newAmmo, sizeof(newAmmo), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)ammo5Addr, &newAmmo, sizeof(newAmmo), nullptr);
	}

	// Health
	if (bHealth)
	{
		// 01 B7 A8010000 = add [edi+000001A8],esi
		mem::PatchEx((BYTE*)(moduleBaseAddr + 0x429E69), (BYTE*)("\x01\xB7\xA8\x01\x00\x00"), 6, hProcess);
	}
	else
	{
		// 29 B7 A8010000 = sub [edi+000001A8],esi     
		mem::PatchEx((BYTE*)(moduleBaseAddr + 0x429E69), (BYTE*)("\x29\xB7\xA8\x01\x00\x00"), 6, hProcess);
	}

	// Fire Rate
	if (bFireRate)
	{
		// 90 = nop
		mem::NopEx((BYTE*)(moduleBaseAddr + 0x54B4CA), 2, hProcess);
	}
	else
	{
		// 89 03 = mov [ebx],eax
		mem::PatchEx((BYTE*)(moduleBaseAddr + 0x54B4CA), (BYTE*)("\x89\x03"), 2, hProcess);
	}

	// Recoil not so recoil
	if (bRecoil)
	{
		// Cursor size increment nop //
		int crossHairSize = 0;

		// F3 0F11 8E 88050000 - movss [esi+00000588],xmm1 (general)
		mem::NopEx((BYTE*)(moduleBaseAddr + 0x1BB552), 8, hProcess);

		// F3 0F11 86 88050000 - movss [esi+00000588],xmm0 (shooting)
		mem::NopEx((BYTE*)(moduleBaseAddr + 0x54B8F5), 8, hProcess);

		// F3 0F11 86 88050000 - movss [esi+00000588],xmm0 (hit)
		mem::NopEx((BYTE*)(moduleBaseAddr + 0x2675E5), 8, hProcess);

		// D9 96 88050000 - fst dword ptr [esi+00000588] (jump)
		mem::NopEx((BYTE*)(moduleBaseAddr + 0x53B702), 6, hProcess);

		WriteProcessMemory(hProcess, (BYTE*)crossHairAddr, &crossHairSize, sizeof(crossHairSize), nullptr);

		// Animation nop //
		
		// 89 96 80050000 - mov[esi + 00000580], edx (gun shooting animation)
		mem::NopEx((BYTE*)(moduleBaseAddr + 0x54B8A7), 6, hProcess);


	}
	else
	{
		// Cursor size op restore
		mem::PatchEx((BYTE*)(moduleBaseAddr + 0x1BB552), (BYTE*)("\xF3\x0F\x11\x8E\x88\x05\x00\x00"), 8, hProcess);
		mem::PatchEx((BYTE*)(moduleBaseAddr + 0x54B8F5), (BYTE*)("\xF3\x0F\x11\x86\x88\x05\x00\x00"), 8, hProcess);
		mem::PatchEx((BYTE*)(moduleBaseAddr + 0x2675E5), (BYTE*)("\xF3\x0F\x11\x86\x88\x05\x00\x00"), 8, hProcess);
		mem::PatchEx((BYTE*)(moduleBaseAddr + 0x53B702), (BYTE*)("\xD9\x96\x88\x05\x00\x00"), 6, hProcess);
		// Animation restore
		mem::PatchEx((BYTE*)(moduleBaseAddr + 0x54B8A7), (BYTE*)("\x89\x96\x80\x05\x00\x00"), 6, hProcess);

	}
}