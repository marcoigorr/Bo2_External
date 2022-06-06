#pragma once
#include <Windows.h>
#include "DataTypes.h"

namespace Values
{
	// Process
	extern DWORD procId;
	extern HANDLE hProcess;

	// General
	extern int windowWidth;
	extern int windowHeight;

	// SnapLines
	extern Vec2 lineOrigin;

	// Cheat values
	extern Matrix ViewMatrix;
	extern int entCount;	
}

