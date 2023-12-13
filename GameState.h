#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <string>

#include "GameConfig.h"

class Exits {
public:
	std::string west;
	std::string east;
	std::string north;
	std::string south;
};

class Room {
public:
	std::string id;
	std::string desc;
	Exits exits;
};

class Object {
public:
	std::string id;
	std::string desc;
	std::string room;
};

class Enemy {
public:
	std::string id;
	std::string desc;
	std::string room;
	int aggressiveness;
	std::vector<std::string> killedBy;
};

class Objective {
public:
	std::string type;
	std::vector<std::string> whats;
};

class Player {
public:
	std::string room;
};

class GameState {
public:
	std::vector<Room> rooms;
	std::vector<Object> objects;
	std::vector<Enemy> enemies;
	Objective objective;
	Player player;
};

#endif