#pragma once
#include "Enemy.h"
#include "Golem.h" // Include Golem here so the factory can create a Golem.
#include "Goblin.h"
#include "Demon.h"
class EnemyFactory {
public:
    static Enemy* createEnemy(EnemyType type) {
        if (type == EnemyType::Golem) {
            return new Golem();  // Create and return a Golem
        }
        else if (type == EnemyType::Goblin) {
            return new Goblin();
        }
        else if (type == EnemyType::Demon) {
            return new Demon();
        }
        return nullptr;
    }
};
