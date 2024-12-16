#pragma once
#include "Manager.h"
#include "Enemy.h"
#include "Character.h"
using namespace std;
class DamageManager : public Manager {
private:
	 std::shared_ptr<Character>& player;
	 std::shared_ptr<Character>& guard;
	vector<shared_ptr<Enemy>>& enemies;
public:
	DamageManager( std::shared_ptr<Character>& pl,std::shared_ptr<Character>& gu, std::vector<std::shared_ptr<Enemy>>& en)
		: player(pl), guard(gu), enemies(en) {};
	void notify(const std::string& event, float value);

};