#pragma once
#include <Windows.h>
#include "DataTypes.h"
#include "imgui/imgui.h"

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
}

namespace Colors
{
	// Color define
	extern ImColor white;
	extern ImColor light_grey;
	extern ImColor light_dark_grey;
	extern ImColor blue;

	extern ImVec4 active;
	extern ImVec4 inactive;
}

