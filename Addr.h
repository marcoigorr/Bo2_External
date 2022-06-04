#pragma once
#include <basetsd.h>
#include <string>
#include "Offsets.h"
#include "MemMan.h"

class Addr
{
public:
	uintptr_t aModuleBase;

	class Process
	{
	public:
		uintptr_t aLocalPlayer;
		uintptr_t aEntList;
		uintptr_t aClientGame;
	};

	class ClientGame
	{
	public:
		uintptr_t viewmatrix;
	};

	class Entity
	{
	public:
		class Zombie
		{
		public:
			uintptr_t pos;
			uintptr_t health;
			uintptr_t healthMax;
		};		

		class Player
		{
		public:
			uintptr_t name;

			uintptr_t health;
			uintptr_t healthMax;

			uintptr_t points;
			uintptr_t mag1;
			uintptr_t mag2;
			uintptr_t ammo1;
			uintptr_t ammo2;
			uintptr_t ammo3;
			uintptr_t ammo4;
			uintptr_t ammo5;
			uintptr_t grenade1;

			uintptr_t crosshair;
		};
	};
};

extern Addr* addr;
extern Addr::Process* aProcess;
extern Addr::ClientGame* aClientGame;
extern Addr::Entity::Zombie* aZombie;
extern Addr::Entity::Player* aPlayer;
