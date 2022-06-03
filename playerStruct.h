#pragma once
#include "stdafx.h"

struct PlayerAddr
{
	uintptr_t name;
	uintptr_t health;
	uintptr_t healthMax;
	uintptr_t points;
	uintptr_t primaryMag;
	uintptr_t secondaryMag;
	uintptr_t ammo1;
	uintptr_t ammo2;
	uintptr_t ammo3;
	uintptr_t ammo4;
	uintptr_t ammo5;
	uintptr_t grenades;

	uintptr_t crossHair;
};

struct PlayerValues
{
	char name[16];
	int health;
	int healthMax;
	int points;
	int primaryMag;
	int secondaryMag;
	int ammo1;
	int ammo2;
	int ammo3;
	int ammo4;
	int ammo5;
	int grenades;

	int crossHair;
};

struct PlayerOffsets
{
	std::vector<unsigned int> name = { 0x5534 };
	std::vector<unsigned int> health = { 0x0, 0x1A8 };
	std::vector<unsigned int> healthMax = { 0x0, 0x1AC };
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

	std::vector<unsigned int> viewMatrix = { 0x128 };
};