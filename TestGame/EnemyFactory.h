#pragma once
#include "Enemy.h"
#include "Golem.h" 
#include "Goblin.h"
#include "Demon.h"
#include "Frogman.h"
#include "Sunflower.h"
#include "Spider.h"
#include "Dragon.h"
#include "Boarman.h"
#include "Skeleton.h"
#include "Ghost.h"
#include "Worm.h"
#include "Medusa.h"
#include "Mage.h"
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
        else if (type == EnemyType::Spider) {
            return std::make_shared<Spider>(spawnPosition);
        }
        else if (type == EnemyType::Dragon1) {
            return std::make_shared<Dragon>(spawnPosition,1);
        }
        else if (type == EnemyType::Dragon2) {
            return std::make_shared<Dragon>(spawnPosition, 2);
        }
        else if (type == EnemyType::Dragon3) {
            return std::make_shared<Dragon>(spawnPosition, 3);
        }
        else if (type == EnemyType::Dragon4) {
            return std::make_shared<Dragon>(spawnPosition, 4);
        }
        else if (type == EnemyType::Skeleton) {
            return std::make_shared<Skeleton>(spawnPosition);
        }
        else if (type == EnemyType::Boarman) {
            return std::make_shared<Boarman>(spawnPosition);
        }
        else if (type == EnemyType::Ghost) {
            return std::make_shared<Ghost>(spawnPosition);
        }
        else if (type == EnemyType::Worm) {
            return std::make_shared<Worm>(spawnPosition);
        }
        else if (type == EnemyType::Mage) {
            return std::make_shared<Mage>(spawnPosition);
        }
        else if (type == EnemyType::Medusa) {
            return std::make_shared<Medusa>(spawnPosition);
        }
        return nullptr;
    }
};
