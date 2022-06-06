#pragma once
#include <string>

class Addr
{
public:
	class Process
	{
	public:
		uintptr_t aModuleBase;
		uintptr_t aLocalPlayer;
		uintptr_t aEntList;
		uintptr_t aClientGame;
	};

	class CG
	{
	public:
		uintptr_t viewmatrix;
	};

	class Entity
	{
	public:
		uintptr_t entity;

		class Zombie
		{
		public:
			uintptr_t vecOrigin;
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
extern Addr::CG* aCG;
extern Addr::Entity* aEntity;
extern Addr::Entity::Zombie* aZombie;
extern Addr::Entity::Player* aPlayer;
