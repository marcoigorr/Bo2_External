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
		uintptr_t client_game = 0x0103AC50;
		uintptr_t local_player = 0x1D88290;
		uintptr_t entity_list = 0x01F387A8;
	};

	class ClientGame
	{
	public:
		std::vector<unsigned int> viewmatrix = { 0x128 };
	};

	class Entity 
	{
	public:
		class Zombie // moduleBase + entity_list
		{
		public:
			std::vector<unsigned int> pos = { 0x18 };
			std::vector<unsigned int> health = { 0x1A8 };
			std::vector<unsigned int> healthMax = { 0x1AC };
		};	

		class Player // moduleBase + local_player
		{
		public:
			std::vector<unsigned int> name = { 0x5534 };

			// entity_list + 0x0 + 0x1A8
			std::vector<unsigned int> health = { 0x0 };
			std::vector<unsigned int> healthMax = { 0x0 };

			std::vector<unsigned int> points = { 0x55C8 };
			std::vector<unsigned int> mag1 = { 0x3F0 };
			std::vector<unsigned int> mag2 = { 0x3F8 };
			std::vector<unsigned int> ammo1 = { 0x42C };
			std::vector<unsigned int> ammo2 = { 0x43C };
			std::vector<unsigned int> ammo3 = { 0x434 };
			std::vector<unsigned int> ammo4 = { 0x440 };
			std::vector<unsigned int> ammo5 = { 0x438 };
			std::vector<unsigned int> grenade1 = { 0x430 };

			std::vector<unsigned int> crosshair = { 0x588 };
		};

	};
};

extern Offsets* offsets;
extern Offsets::Process* oProcess;
extern Offsets::ClientGame* oClientGame;
extern Offsets::Entity::Zombie* oZombie;
extern Offsets::Entity::Player* oPlayer;
