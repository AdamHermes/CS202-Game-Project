#include <vector>
#include <memory>
#include "Enemy.h"
#include "Goblin.h"
#include "Golem.h"
#include "Demon.h"
class EnemyPool {
private:
    std::vector<Enemy*> pool;

public:
    // Preallocate enemies based on their type
    void initializePool(EnemyType type, int count) {
        for (int i = 0; i < count; ++i) {
            switch (type) {
            case EnemyType::Golem:
                pool.push_back(new Golem());
                break;
            case EnemyType::Goblin:
                pool.push_back(new Goblin());
                break;
            case EnemyType::Demon:
                pool.push_back(new Demon());
                break;
            }
        }
    }

    // Borrow an enemy from the pool
    Enemy* getEnemy() {
        if (pool.empty()) return nullptr; // Handle case where no enemy is available
        Enemy* enemy = pool.back();
        pool.pop_back();
        return enemy;
    }

    // Return an enemy back to the pool
    void returnEnemy(Enemy* enemy) {
        pool.push_back(enemy);
    }

    ~EnemyPool() {
        for (Enemy* enemy : pool) {
            delete enemy;
        }
    }
};
