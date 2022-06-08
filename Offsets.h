#pragma once
#include <basetsd.h>
#include <string>
#include <vector>

class Offsets
{
public:
	class Process 
	{
	public:
		uintptr_t client_game = 0x159D34;
		uintptr_t local_player = 0x1D88290;
		uintptr_t entity_list = 0x01F387A8;
	};

	class CG
	{
	public:
		std::vector<unsigned int> viewmatrix = { 0xf0 };
	};

	class Entity 
	{
	public:
		class Zombie // moduleBase + entity_list
		{
		public:
			std::vector<unsigned int> vecOrigin = { 0x18 };
			std::vector<unsigned int> health = { 0x1A8 };
			std::vector<unsigned int> healthMax = { 0x1AC };
		};	

		class Player // moduleBase + local_player
		{
		public:
			std::vector<unsigned int> name = { 0x5534 };

			std::vector<unsigned int> speed = { 0x94 };
			std::vector<unsigned int> gravity = { 0x8C };
			// entity_list + 0x0 + 0x1A8
			std::vector<unsigned int> health = { 0x0 };
			std::vector<unsigned int> healthMax = { 0x0 };

			std::vector<unsigned int> points = { 0x55C8 };
			std::vector<unsigned int> mag1 = { 0x3F0 };
			std::vector<unsigned int> mag2 = { 0x3EC };
			std::vector<unsigned int> ammo1 = { 0x42C };
			std::vector<unsigned int> ammo2 = { 0x438 };
			std::vector<unsigned int> ammo3 = { 0x428 };
			std::vector<unsigned int> ammo4 = { 0x440 };
			std::vector<unsigned int> grenade1 = { 0x430 };
			std::vector<unsigned int> grenade2 = { 0x43C };

			std::vector<unsigned int> crosshair = { 0x588 };
		};

	};
};

extern Offsets* offsets;
extern Offsets::Process* oProcess;
extern Offsets::CG* oCG;
extern Offsets::Entity::Zombie* oZombie;
extern Offsets::Entity::Player* oPlayer;
