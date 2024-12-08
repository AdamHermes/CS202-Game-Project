#pragma once
#include "Enemy.h"
#include "Golem.h" // Include Golem here so the factory can create a Golem.
#include "Goblin.h"
#include "Demon.h"
#include "Frogman.h"
#include "Sunflower.h"
class EnemyFactory {
public:
    static std::shared_ptr<Enemy> createEnemy(EnemyType type, sf::Vector2f spawnPosition) {
        if (type == EnemyType::Golem) {
            return std::make_shared<Golem>(spawnPosition); 
        }
        else if (type == EnemyType::Goblin) {
            return std::make_shared<Goblin>(spawnPosition); 
        }
        else if (type == EnemyType::Demon) {
            return std::make_shared<Demon>(spawnPosition);
        }
        else if (type == EnemyType::Frogman) {
            return std::make_shared<Frogman>(spawnPosition);
        }
        else if (type == EnemyType::Sunflower) {
            return std::make_shared<Sunflower>(spawnPosition);

        }
        return nullptr;
    }
};
