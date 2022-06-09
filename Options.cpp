#include "Options.h"

namespace Options
{
	bool bCalcAddr = false;

	// menu
	bool bMenu = false;

	// health
	bool bHealth = false;
	int iHealth = 100;

	// gravity (player)
	bool bGravity = false;
	int iGravity = 800; // default

	// speed
	bool bSpeedHack = false;
	int iSpeed = 190; // default

	// points
	bool bPoints = false;
	int iPoints = 500;
	bool bFreezePoints = false;

	// ammo
	bool bAmmo = false;
	int iAmmo = 420;
	int iGrenade = 3;

	// fire rate
	bool bFireRate = false;

	// recoil
	bool bRecoil = false;

	// ESP
	bool bESP = false;	
	bool bSnapLines = false;

	// other
	int iZombieCount = 0;
}