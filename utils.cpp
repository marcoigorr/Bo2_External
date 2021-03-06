#pragma once
#include "utils.h"
using namespace Values;
using namespace Options;

#define aModuleBase aProcess->aModuleBase
#define aLocalPlayer aProcess->aLocalPlayer
#define aEntList aProcess->aEntList
#define aClientGame aProcess->aClientGame
#define entity aEntity->entity

// plutonium-bootstrapper-win32.exe+40F64D - F3 A5 - repe movsd (strange fire glitch) 

void GetGameAddr()
{
	aPlayer->speed = aLocalPlayer + oPlayer->speed[0];
	aPlayer->gravity = aLocalPlayer + oPlayer->gravity[0];
	aPlayer->mag1 = aLocalPlayer + oPlayer->mag1[0];
	aPlayer->mag2 = aLocalPlayer + oPlayer->mag2[0];
	aPlayer->ammo1 = aLocalPlayer + oPlayer->ammo1[0];
	aPlayer->ammo2 = aLocalPlayer + oPlayer->ammo2[0];
	aPlayer->ammo3 = aLocalPlayer + oPlayer->ammo3[0];
	aPlayer->ammo4 = aLocalPlayer + oPlayer->ammo4[0];
	aPlayer->grenade1 = aLocalPlayer + oPlayer->grenade1[0];
	aPlayer->grenade2 = aLocalPlayer + oPlayer->grenade2[0];
	aPlayer->points = aLocalPlayer + oPlayer->points[0];
	aPlayer->crosshair = aLocalPlayer + oPlayer->crosshair[0];
}

/* ----------- ImGui Menu ----------- */
void updateMenu(GLFWwindow* window)
{
	MemMan mem;

	// Calculate Process Addresses
	aLocalPlayer = mem.ReadMem<uintptr_t>(hProcess, aModuleBase + oProcess->local_player); // 0x023427A0
	aEntList = mem.ReadMem<uintptr_t>(hProcess, aModuleBase + oProcess->entity_list); // 0x2330358
	aClientGame = mem.ReadMem<uintptr_t>(hProcess, aModuleBase + oProcess->client_game); // 0x1AB10107	
	
	GetGameAddr();

	// ImGui menu
	if (bMenu)
	{
		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);
	}
	else
	{
		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, true);
	}

	// --- ESP 
	ViewMatrix = mem.ReadMem<Matrix>(hProcess, aClientGame + oCG->viewmatrix[0]);

	// Loop Through ent list (fisrt element is player)      
	iZombieCount = 0;
	for (short int i = 1; mem.ReadMem<uintptr_t>(hProcess, aEntList + i * 0x8C) != 0x0; i++)
	{
		entity = mem.ReadMem<uintptr_t>(hProcess, aEntList + i * 0x8C);

		// If current ent is not alive restart
		if (mem.ReadMem<int>(hProcess, entity + oZombie->health[0]) <= 0) continue;
		iZombieCount++;		

		// instant kill
		if (bInstantKill)
		{
			int x = 1;
			WriteProcessMemory(hProcess, (BYTE*)(entity + oZombie->health[0]), &x, sizeof(x), nullptr);
		}

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

	// --- FireRate
	if (bFireRate)
	{
		// 90 = nop
		mem.NopEx((BYTE*)(aModuleBase + 0x54B4CA), 2, hProcess);
	}
	else
	{
		// 89 03 = mov [ebx],eax
		mem.PatchEx((BYTE*)(aModuleBase + 0x54B4CA), (BYTE*)("\x89\x03"), 2, hProcess);
	}

	// --- Recoil not so recoil
	if (bRecoil)
	{
		// Cursor size increment nop //
		int crossHairSize = 0;

		// F3 0F11 8E 88050000 - movss [esi+00000588],xmm1 (general)
		mem.NopEx((BYTE*)(aModuleBase + 0x1BB552), 8, hProcess);

		// F3 0F11 86 88050000 - movss [esi+00000588],xmm0 (shooting)
		mem.NopEx((BYTE*)(aModuleBase + 0x54B8F5), 8, hProcess);

		// F3 0F11 86 88050000 - movss [esi+00000588],xmm0 (hit)
		mem.NopEx((BYTE*)(aModuleBase + 0x2675E5), 8, hProcess);

		// D9 96 88050000 - fst dword ptr [esi+00000588] (jump)
		mem.NopEx((BYTE*)(aModuleBase + 0x53B702), 6, hProcess);

		WriteProcessMemory(hProcess, (BYTE*)aPlayer->crosshair, &crossHairSize, sizeof(crossHairSize), nullptr);

		// Animation nop //

		// 89 96 80050000 - mov[esi + 00000580], edx (gun shooting animation)
		mem.NopEx((BYTE*)(aModuleBase + 0x54B8A7), 6, hProcess);
	}
	else
	{
		// Cursor size op restore
		mem.PatchEx((BYTE*)(aModuleBase + 0x1BB552), (BYTE*)("\xF3\x0F\x11\x8E\x88\x05\x00\x00"), 8, hProcess);
		mem.PatchEx((BYTE*)(aModuleBase + 0x54B8F5), (BYTE*)("\xF3\x0F\x11\x86\x88\x05\x00\x00"), 8, hProcess);
		mem.PatchEx((BYTE*)(aModuleBase + 0x2675E5), (BYTE*)("\xF3\x0F\x11\x86\x88\x05\x00\x00"), 8, hProcess);
		mem.PatchEx((BYTE*)(aModuleBase + 0x53B702), (BYTE*)("\xD9\x96\x88\x05\x00\x00"), 6, hProcess);
		// Animation restore
		mem.PatchEx((BYTE*)(aModuleBase + 0x54B8A7), (BYTE*)("\x89\x96\x80\x05\x00\x00"), 6, hProcess);
	}

	// --- Health
	if (bHealth)
	{
		// 01 B7 A8010000 = add [edi+000001A8],esi
		mem.PatchEx((BYTE*)(aModuleBase + 0x429E69), (BYTE*)("\x01\xB7\xA8\x01\x00\x00"), 6, hProcess);
	}
	else
	{
		// 29 B7 A8010000 = sub [edi+000001A8],esi     
		mem.PatchEx((BYTE*)(aModuleBase + 0x429E69), (BYTE*)("\x29\xB7\xA8\x01\x00\x00"), 6, hProcess);

		//89 81 A8010000 = mov[ecx + 000001A8], eax
		//mem.PatchEx((BYTE*)(aModuleBase + 0x2872C6), (BYTE*)("\x89\x81\xA8\x01\x00\x00"), 6, hProcess);
	}

	// --- Speed
	if (bSpeedHack)
	{
		WriteProcessMemory(hProcess, (BYTE*)aPlayer->speed, &iSpeed, sizeof(iSpeed), nullptr);
		mem.NopEx((BYTE*)(aModuleBase + 0x34EE1), 6, hProcess);
	}
	else
	{
		// 89 85 94000000 = mov [ebp+00000094],eax (nop this and set aLocalPlayer + 0x94 with the desired speed)
		mem.PatchEx((BYTE*)(aModuleBase + 0x34EE1), (BYTE*)("\x89\x85\x94\x00\x00\x00"), 6, hProcess);
	}

	// --- Points
	if (bPoints)
	{		
		bPoints = false;
		int newPonts = mem.ReadMem<int>(hProcess, aPlayer->points) + iPoints;
		WriteProcessMemory(hProcess, (BYTE*)aPlayer->points, &newPonts, sizeof(newPonts), nullptr);
	}
	if (bFreezePoints)
	{
		mem.NopEx((BYTE*)(aModuleBase + 0x41FC18), 6, hProcess);
		mem.NopEx((BYTE*)(aModuleBase + 0x41FBE5), 6, hProcess);
	}
	else
	{
		// 41FC18 - 89 B3 C8550000 = mov [ebx+000055C8],esi
		mem.PatchEx((BYTE*)(aModuleBase + 0x41FC18), (BYTE*)("\x89\xB3\xC8\x55\x00\x00"), 6, hProcess);
		// 89 B8 D8000000 = mov [eax+000000D8],edi
		mem.PatchEx((BYTE*)(aModuleBase + 0x41FBE5), (BYTE*)("\x89\xB8\xD8\x00\x00\x00"), 6, hProcess);
	}

	// --- Ammo
	if (bAmmo)
	{
		//Write to memory ammo
		WriteProcessMemory(hProcess, (BYTE*)aPlayer->ammo1, &iAmmo, sizeof(iAmmo), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)aPlayer->ammo2, &iAmmo, sizeof(iAmmo), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)aPlayer->ammo3, &iAmmo, sizeof(iAmmo), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)aPlayer->ammo4, &iAmmo, sizeof(iAmmo), nullptr);

		WriteProcessMemory(hProcess, (BYTE*)aPlayer->grenade1, &iGrenade, sizeof(iGrenade), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)aPlayer->grenade2, &iGrenade, sizeof(iGrenade), nullptr);
	}

	// --- Gravity (player)
	if (bGravity)
	{		
		WriteProcessMemory(hProcess, (BYTE*)aPlayer->gravity, &iGravity, sizeof(iGravity), nullptr);
		mem.NopEx((BYTE*)(aModuleBase + 0x16CC86), 6, hProcess);
	}
	else
	{
		// 89 85 8C000000 = mov[ebp+0000008C],eax
		mem.PatchEx((BYTE*)(aModuleBase + 0x16CC86), (BYTE*)("\x89\x85\x8C\x00\x00\x00"), 6, hProcess);
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
