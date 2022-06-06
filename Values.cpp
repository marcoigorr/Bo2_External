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
	int entCount = 0;
	
}
