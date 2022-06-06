#pragma once
#include "utils.h"
using namespace Values;
using namespace Options;

#define aModuleBase aProcess->aModuleBase
#define aLocalPlayer aProcess->aLocalPlayer
#define aEntList aProcess->aEntList
#define aClientGame aProcess->aClientGame
#define entity aEntity->entity


/* ----------- ImGui Menu ----------- */
void updateMenu(GLFWwindow* window)
{
	MemMan mem;

	if (bMenu)
	{
		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);
	}
	else
	{
		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, true);
	}

	// --- ESP 
	if (bESP)
	{
		ViewMatrix = mem.ReadMem<Matrix>(hProcess, aClientGame + oCG->viewmatrix[0]);

		// Loop Through ent list (fisrt element is player)      
		entCount = 0;
		for (short int i = 1; mem.ReadMem<uintptr_t>(hProcess, aEntList + i * 0x8C) != 0x0; i++)
		{
			entity = mem.ReadMem<uintptr_t>(hProcess, aEntList + i * 0x8C);

			// If current ent is not alive restart
			if (mem.ReadMem<int>(hProcess, entity + oZombie->health[0]) <= 0) continue;
			entCount++;

			// Snap Lines
			if (bSnapLines)
			{
				Vec3 entLocation;
				entLocation = mem.ReadMem<Vec3>(hProcess, entity + oZombie->vecOrigin[0]);

				Vec2 ScreenCoords;
				if (!WorldToScreen(entLocation, ScreenCoords, ViewMatrix.matrix)) continue;

				DrawLine(lineOrigin, ScreenCoords);
			}
		}
	}
}

void DrawLine(Vec2& start, Vec2& end)
{
	glBegin(GL_LINES);
	glColor4f(0, 1, 0, 2);
	glVertex2d(start.X, start.Y);
	glVertex2d(end.X, end.Y);
	glEnd();
}

/* ----------- ESP ----------- */
void ConvertToRange(Vec2& Point)
{
	Point.X /= Values::windowWidth;
	Point.X *= 2.0f;
	Point.X -= 1.0f;

	Point.Y /= Values::windowHeight;
	Point.Y *= 2.0f;
	Point.Y -= 1.0f;
}

bool WorldToScreen(const Vec3& VecOrigin, Vec2& VecScreen, float* Matrix)
{
	VecScreen.X = VecOrigin.X * Matrix[0] + VecOrigin.Y * Matrix[1] + VecOrigin.Z * Matrix[2] + Matrix[3];
	VecScreen.Y = VecOrigin.X * Matrix[4] + VecOrigin.Y * Matrix[5] + VecOrigin.Z * Matrix[6] + Matrix[7];
	float W = VecOrigin.X * Matrix[12] + VecOrigin.Y * Matrix[13] + VecOrigin.Z * Matrix[14] + Matrix[15];

	if (W < 0.01f)
		return false;

	// Normalized Device Coordinates
	Vec2 NDC;
	NDC.X = VecScreen.X / W;
	NDC.Y = VecScreen.Y / W;

	VecScreen.X = (Values::windowWidth / 2 * NDC.X) + (NDC.X + Values::windowWidth / 2);
	VecScreen.Y = (Values::windowHeight / 2 * NDC.Y) + (NDC.Y + Values::windowHeight / 2);

	ConvertToRange(VecScreen);

	return true;
}
