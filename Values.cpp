#include "Values.h"

namespace Values
{
	// Process
	DWORD procId;
	HANDLE hProcess = 0;

	// General
	int windowWidth;
	int windowHeight;

	//SnapLines
	Vec2 lineOrigin = Vec2(0.0f, -1.0f);

	// Cheat values
	Matrix ViewMatrix;	
}

namespace Colors
{
	ImColor white = ImColor(255, 255, 255);
	ImColor light_grey = ImColor(16, 16, 16);
	ImColor light_dark_grey = ImColor(24, 24, 24);
	ImColor blue = ImColor(50, 130, 200);

	ImVec4 active = ImVec4(50 / 255.0, 130 / 255.0, 200 / 255.0, 255 / 255.0);
	ImVec4 inactive = ImVec4(31 / 255.0, 30 / 255.0, 31 / 255.0, 255 / 255.0);
}
