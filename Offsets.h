#pragma once
#include <basetsd.h>
#include <string>

class Offsets
{
public:
	class Process 
	{
	public:
		int client_game = 0x0103AC50;
		int local_player = 0x1D88290;
		int entity_list = 0x0103AC50;
	};

	class ClientGame
	{
	public:
		int viewmatrix = 0x128;
	};

	class Entity // moduleBase + entity_list
	{
	public:
		int pos = 0x18;
		int health = 0x1A8;
		int healthMax = 0x1AC;

		class Player // moduleBase + local_player
		{
		public:
			int name = 0x5534;

			// entity_list + 0x0 + 0x1A8
			int health = 0x0;
			int healthMax = 0x0;

			int points = 0x55C8;
			int mag1 = 0x3F0;
			int mag2 = 0x3F8;
			int ammo1 = 0x42C;
			int ammo2 = 0x43C;
			int ammo3 = 0x434;
			int ammo4 = 0x440;
			int ammo5 = 0x438;
			int grenade1 = 0x430;

			int crosshair = 0x588;
		};

	};

public:
	auto get_local_player()->DWORD_PTR;
	auto get_entity_list()->DWORD_PTR;
	auto is_entity_dead(DWORD_PTR dw_entity) -> bool;
};

extern Offsets* offsets;
extern Offsets::Process* p_process;
extern Offsets::ClientGame* p_cg;
extern Offsets::Entity* p_entity;
extern Offsets::Entity::Player* p_player;
