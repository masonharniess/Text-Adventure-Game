#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <iostream>

#include "GameState.h"

class GameEngine
{

public:
	GameEngine(GameState& state): state(state){
		shouldExit = false;
		roomChanged = true;
		isPlayerAlive = true;
		
		currentRoom = NULL;

		goToRoom(state.player.room);
	}
	
	void start();

private:
	GameState& state;

	std::vector<Object> playerItems;

	Room* currentRoom;
	bool roomChanged;

	bool isPlayerAlive;

	bool shouldExit;
	std::string reasonForDeath;

	void initalise();
	void run();

	void print(const std::string& text, bool line=true);
	void printCurrentRoom();
	void runCurrentRoom();

	std::string input(const std::string& text);

	bool handleCommand(const std::string& text);

	void goToDirection(const std::string& direction);
	void goToRoom(const std::string& room);

	std::vector<Object*> getRoomObjects(const std::string& roomId);
	void lookAtObject(const std::string& objectId);

	void pickUpObject(const std::string& objectId);

	std::vector<Enemy*> getRoomEnemies(const std::string& roomId);
	void fightEnemy(const std::string& enemyId);

	void killEnemy(const std::string& enemyId);

	void aggroEnemy();
};

#endif