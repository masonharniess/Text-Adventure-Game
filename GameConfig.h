#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <vector>
#include <string>

#include "json.hpp"

namespace ns {
	class exits_config {
	public:
		std::string west;
		std::string east;
		std::string north;
		std::string south;
	};

	class room_config {
	public:
		std::string id;
		std::string desc;
		exits_config exists;
	};

	class object_config {
	public:
		std::string id;
		std::string desc;
		std::string room;
	};

	class enemy_config {
	public:
		std::string id;
		std::string desc;
		std::string room;
		int aggressiveness;
		std::vector<std::string> killed_by;
	};

	class player_config {
	public:
		std::string room;
	};

	class objective_config {
	public:
		std::string type;
		std::vector<std::string> whats;
	};

	class game_config {
	public:
		std::vector<room_config> rooms;
		std::vector<object_config> objects;
		std::vector<enemy_config> enemies;
		objective_config objective;
		player_config player;
	};

	
}

#endif 
