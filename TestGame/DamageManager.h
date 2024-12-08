#pragma once
#include "Manager.h"
#include "Enemy.h"
#include "Character.h"
using namespace std;
class DamageManager : public Manager {
private:
	Character& player;
	vector<shared_ptr<Enemy>>& enemies;
public:
	DamageManager(Character& pl, std::vector<std::shared_ptr<Enemy>>& en)
		: player(pl), enemies(en) {};
	void notify(const std::string& event, float value);

};