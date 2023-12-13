#include <fstream>

#include "GameEngine.h"
#include "GameConfig.cpp"

namespace ns {
	void from_json(const nlohmann::json& j, exits_config& e) {
		if (j.contains("east")) j.at("east").get_to(e.east);
		if (j.contains("west")) j.at("west").get_to(e.west);
		if (j.contains("north")) j.at("north").get_to(e.north);
		if (j.contains("north")) j.at("north").get_to(e.south);
	}


	void from_json(const nlohmann::json& j, room_config& r) {
		j.at("id").get_to(r.id);
		j.at("desc").get_to(r.desc);
		j.at("exits").get_to(r.exists);
	}

	void from_json(const nlohmann::json& j, object_config& r) {
		j.at("id").get_to(r.id);
		j.at("desc").get_to(r.desc);
		j.at("initialroom").get_to(r.room);
	}

	void from_json(const nlohmann::json& j, enemy_config& r) {
		j.at("id").get_to(r.id);
		j.at("desc").get_to(r.desc);
		j.at("initialroom").get_to(r.room);
		j.at("aggressiveness").get_to(r.aggressiveness);
		j.at("killedby").get_to(r.killed_by);
	}

	void from_json(const nlohmann::json& j, objective_config& r) {
		j.at("type").get_to(r.type);
		j.at("what").get_to(r.whats);
	}

	void from_json(const nlohmann::json& j, player_config& r) {
		j.at("initialroom").get_to(r.room);
	}

	void from_json(const nlohmann::json& j, game_config& g) {
		j.at("rooms").get_to(g.rooms);
		j.at("objects").get_to(g.objects);
		j.at("enemies").get_to(g.enemies);
		j.at("objective").get_to(g.objective);
		j.at("player").get_to(g.player);
	}
}

static Exits from_config(ns::exits_config& e) {
	Exits exits;

	exits.east = e.east;
	exits.north = e.north;
	exits.south = e.south;
	exits.west = e.west;

	return exits;
}

static Room from_config(ns::room_config& r) {
	Room room;

	room.desc = r.desc;
	room.id = r.id;
	room.exits = from_config(r.exists);

	return room;
}

static Object from_config(ns::object_config& r) {
	Object room;

	room.desc = r.desc;
	room.id = r.id;
	room.room = r.room;

	return room;
}

static Enemy from_config(ns::enemy_config& r) {
	Enemy enemy;

	enemy.id = r.id;
	enemy.desc = r.desc;
	enemy.room = r.room;
	enemy.aggressiveness = r.aggressiveness;
	enemy.killedBy = r.killed_by;

	return enemy;
}

static Objective from_config(ns::objective_config& r) {
	Objective objective;

	objective.type = r.type;
	objective.whats = r.whats;

	return objective;
}

static Player from_config(ns::player_config& r) {
	Player player;

	player.room = r.room;

	return player;
}

static GameState from_config(ns::game_config& g) {
	GameState state;

	for (auto& room : g.rooms) {
		state.rooms.push_back(from_config(room));
	}

	for (auto& object : g.objects) {
		state.objects.push_back(from_config(object));
	}

	for (auto& enemy : g.enemies) {
		state.enemies.push_back(from_config(enemy));
	}

	state.objective = from_config(g.objective);

	state.player = from_config(g.player);

	return state;
}

int main(int argc, char* argv[]) {
	if (argc < 1) return -1;

	std::string mapFile = argv[1];

	std::ifstream f(mapFile);
	nlohmann::json data = nlohmann::json::parse(f);

	auto gam1 = data.get<ns::game_config>();

	GameState gameState = from_config(gam1);
	GameEngine engine(gameState);

	srand(time(NULL));

	engine.start();

	return 0;
}