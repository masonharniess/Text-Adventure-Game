#include "GameEngine.h"

void GameEngine::start()
{
	initalise();
	
	while(!shouldExit)run();
}

void GameEngine::initalise()
{
}

void GameEngine::run()
{
	if (!isPlayerAlive) {
		print("");
		print("You Died!");
		print(reasonForDeath);
		print("Game Over!");
		shouldExit = true;
		return;
	}

	if (state.objective.whats.size() < 1) {
		print("");
		print("You have completed the objective");
		shouldExit = true;
		return;
	}

	if (roomChanged) {
		runCurrentRoom();
		roomChanged = false;
		return;
	}

	std::string command = input("> ");
	handleCommand(command);
}

void GameEngine::print(const std::string& text, bool line) {
	std::cout << text;
	if (line) std::cout << std::endl;
}

void GameEngine::printCurrentRoom() {
	//if (this->currentRoomIndex >= state.rooms.size()) return;
	print("You entered " + currentRoom->id);
	print(currentRoom->desc);
	
	std::vector<Object*> objects = getRoomObjects(currentRoom->id);

	if (objects.size() >= 1) {
		print("");
		print("Room Contains: ");

		for (int i = 0; i < objects.size(); ++i) {
			print("* " + objects[i]->id);
		}
	}
	

	std::vector<Enemy*> enemies = getRoomEnemies(currentRoom->id);
	if (enemies.size() < 1) return;
	print("");
	print("Enemies in the room: ");

	for (int i = 0; i < enemies.size(); ++i) {
		print("* " + enemies[i]->id);
	}
}

void GameEngine::runCurrentRoom() {
	printCurrentRoom();
}

std::string GameEngine::input(const std::string& text)
{
	print(text, false);
	std::string input;
	std::getline(std::cin, input);
	return input;
}

bool GameEngine::handleCommand(const std::string& text)
{
	std::vector<std::string> tokens;
	std::string token = "";
	int startIndex = 0;
	int length = 0;
	for (int i = 0; i < text.size(); ++i) {
		if (text[i] == ' ') {
			tokens.push_back(text.substr(startIndex, length));
			length = 0;
			startIndex = i + 1;
			continue;
		}
		++length;
	}

	tokens.push_back(text.substr(startIndex, length));

	if (tokens.size() < 1) return false;
	
	std::string command = tokens[0];

	if (command == "look") {

		if (tokens.size() <= 1) { printCurrentRoom(); return true; }
		if (tokens.size() == 2 && tokens[1] == "around") { printCurrentRoom(); return true; }

		std::string objectName = "";

		for (int i = 1; i < tokens.size(); ++i) {
			objectName += tokens[i];
			if (i < tokens.size()-1) objectName += " ";
		}

		lookAtObject(objectName);
		
		return true;
	}else if (command == "go") {
		if (tokens.size() <= 1) return false;

		std::string& direction = tokens[1];

		goToDirection(direction);
		return true;
	}else if (command == "take") {
		if (tokens.size() <= 1) return false;

		std::string objectName = "";

		for (int i = 1; i < tokens.size(); ++i) {
			objectName += tokens[i];
			if (i < tokens.size() - 1) objectName += " ";
		}

		pickUpObject(objectName);
		return true;
	}else if (command == "kill") {
		if (tokens.size() <= 1) return false;

		std::string enemyName = "";

		for (int i = 1; i < tokens.size(); ++i) {
			enemyName += tokens[i];
			if (i < tokens.size() - 1) enemyName += " ";
		}

		fightEnemy(enemyName);
		return true;
	}
	else if (command == "list") {
		if (playerItems.size() < 1) { print("You have no items"); return true; }

		print("You have: ");

		for (int i = 0; i < playerItems.size(); ++i) {
			std::cout << "* " << playerItems[i].id << std::endl;
		}
		return true;
	}
	
	return false;
}

void GameEngine::goToDirection(const std::string& direction)
{
	std::string roomId = "";
	if (direction == "east") {
		roomId = currentRoom->exits.east;
	}else if (direction == "west") {
		roomId = currentRoom->exits.west;
	}else if (direction == "south") {
		roomId = currentRoom->exits.south;
	}else if (direction == "north") {
		roomId = currentRoom->exits.north;
	}

	if (roomId.size() > 1) {
		aggroEnemy();
		if (!isPlayerAlive) return;

		goToRoom(roomId);
	}
	else print("Invalid direction or door doesn't exist on that direction");
}

void GameEngine::goToRoom(const std::string& roomId) {

	for (int i = 0; i < state.rooms.size(); ++i) {
		Room& room = state.rooms[i];
		if (room.id == roomId) { currentRoom = &room; roomChanged = true; break; }
	}

	if (roomChanged && state.objective.type == "room") {
		int indexToRemove = -1;
		for (int i = 0; i < state.objective.whats.size(); ++i) {
			if (state.objective.whats[i] == roomId) {
				indexToRemove = i;
			}
		}
		if (indexToRemove > -1)
			state.objective.whats.erase(state.objective.whats.begin() + indexToRemove);
	}
}

std::vector<Object*> GameEngine::getRoomObjects(const std::string& roomId)
{
	std::vector<Object*> roomObjects;
	for (Object& object : state.objects) {
		if(object.room == roomId)roomObjects.push_back(&object);
	}
	return roomObjects;
}

void GameEngine::lookAtObject(const std::string& objectId) {

	for (int i = 0; i < playerItems.size(); ++i) {
		if (objectId == playerItems[i].id) {
			print(playerItems[i].desc);
			return;
		}
	}

	std::vector<Object*> roomObjects = getRoomObjects(currentRoom->id);

	for (int i = 0; i < roomObjects.size(); ++i) {
		if (objectId == roomObjects[i]->id) {
			print(roomObjects[i]->desc);
			return;
		}
	}
}

void GameEngine::pickUpObject(const std::string& objectId)
{
	std::vector<Object*> roomObjects = getRoomObjects(currentRoom->id);

	bool objectFound = false;

	for (int i = 0; i < roomObjects.size(); ++i) {
		if (objectId == roomObjects[i]->id) {
			playerItems.push_back(*roomObjects[i]);
			objectFound = true;
			break;
		}
	}
	if (!objectFound) return;

	int indexToRemove = -1;
	for (int i = 0; i < state.objects.size(); ++i) {
		if (objectId == (state.objects[i]).id) indexToRemove = i;
	}

	state.objects.erase(state.objects.begin()+indexToRemove);

	if (state.objective.type == "collect") {
		int indexToRemove = -1;
		for (int i = 0; i < state.objective.whats.size(); ++i) {
			if (state.objective.whats[i] == objectId) {
				indexToRemove = i;
			}
		}
		if (indexToRemove > -1)
			state.objective.whats.erase(state.objective.whats.begin() + indexToRemove);
	}

	print("Picked up " + objectId);
}

std::vector<Enemy*> GameEngine::getRoomEnemies(const std::string& roomId)
{
	std::vector<Enemy*> roomEnemies;
	for (Enemy& enemy : state.enemies) {
		if (enemy.room == roomId)roomEnemies.push_back(&enemy);
	}
	return roomEnemies;
}

void GameEngine::killEnemy(const std::string& enemyId) {
	int indexToRemove = -1;
	for (int i = 0; i < state.enemies.size(); ++i) {
		if (enemyId == (state.enemies[i]).id) indexToRemove = i;
	}

	state.enemies.erase(state.enemies.begin() + indexToRemove);

	print("Killed " + enemyId);
}

void GameEngine::fightEnemy(const std::string& enemyId)
{
	std::vector<Enemy*> roomEnemies = getRoomEnemies(currentRoom->id);

	Enemy* enemyToFight = NULL;

	for (int i = 0; i < roomEnemies.size(); ++i) {
		if (enemyId == roomEnemies[i]->id) {
			enemyToFight = roomEnemies[i];
			break;
		}
	}

	if (enemyToFight == NULL) return;

	bool canPlayerKill = true;

	std::vector<std::string> killedBy = enemyToFight->killedBy;

	for (std::string& killedB : killedBy) {
		bool foundItem = false;
		for (Object& playerObject : playerItems) {
			if (playerObject.id == killedB) { foundItem = true; break; }
		}
		if (!foundItem) { canPlayerKill = false; break; }
	}

	if (canPlayerKill) {
		killEnemy(enemyId);
		
		if(state.objective.type == "kill"){
			int indexToRemove = -1;
			for (int i = 0; i < state.objective.whats.size(); ++i) {
				if (state.objective.whats[i] == enemyId) {
					indexToRemove = i;
				}
			}
			if(indexToRemove > -1)
				state.objective.whats.erase(state.objective.whats.begin()+indexToRemove);
		}
	}
	else {
		isPlayerAlive = false;
		reasonForDeath = "You didn't have the necessary items to kill the enemy";
	}
}

void GameEngine::aggroEnemy() {
	
	std::vector<Enemy*> roomEnemies = getRoomEnemies(currentRoom->id);

	if (roomEnemies.size() < 1) return;

	for (int i = 0; i < roomEnemies.size(); ++i) {
		Enemy* enemy = roomEnemies[i];

		int randomNum = rand() % 100+1;
		std::cout << randomNum << std::endl;

		if (randomNum <= enemy->aggressiveness) {
			isPlayerAlive = false;
			reasonForDeath = "You were killed by the enemy while leaving the room";
			break;
		}
	}
}

