#pragma once
#include <Windows.h>
#include <vector>

class MemMan
{
private:
	HANDLE handle;

public:
	MemMan();
	~MemMan();

	template <class T>
	static T ReadMem(HANDLE hProcess, uintptr_t addr)
	{
		T x;
		ReadProcessMemory(hProcess, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	template <class T>
	static T WriteMem(HANDLE hProcess, uintptr_t addr, T x)
	{
		WriteProcessMemory(hProcess, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
};

