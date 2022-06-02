#pragma once
#include "stdafx.h"

struct PlayerAddr
{
	uintptr_t name = 0;
	uintptr_t health = 0;
	uintptr_t healthMax = 0;
	uintptr_t points = 0;
	uintptr_t primaryMag = 0;
	uintptr_t secondaryMag = 0;
	uintptr_t ammo1 = 0;
	uintptr_t ammo2 = 0;
	uintptr_t ammo3 = 0;
	uintptr_t ammo4 = 0;
	uintptr_t ammo5 = 0;
	uintptr_t grenades = 0;

	uintptr_t crossHair = 0;
};

struct PlayerValues
{
	char name[16] = "Game Name Here";
	int health = 0;
	int healthMax = 0;
	int points = 0;
	int primaryMag = 0;
	int secondaryMag = 0;
	int ammo1 = 0;
	int ammo2 = 0;
	int ammo3 = 0;
	int ammo4 = 0;
	int ammo5 = 0;
	int grenades = 0;

	int crossHair = 0;
};

struct PlayerOffsets
{
	std::vector<unsigned int> name = { 0x5534 };
	std::vector<unsigned int> health = { 0x1A8 };
	std::vector<unsigned int> healthMax = { 0x1AC };
	std::vector<unsigned int> points = { 0x55C8 };
	std::vector<unsigned int> primaryMag = { 0x3F0 };
	std::vector<unsigned int> secondaryMag = { 0x3F8 };
	std::vector<unsigned int> ammo1 = { 0x42C };
	std::vector<unsigned int> ammo2 = { 0x43C };
	std::vector<unsigned int> ammo3 = { 0x434 };
	std::vector<unsigned int> ammo4 = { 0x440 };
	std::vector<unsigned int> ammo5 = { 0x438 };
	std::vector<unsigned int> grenades = { 0x430 };

	std::vector<unsigned int> crossHair = { 0x588 };
};