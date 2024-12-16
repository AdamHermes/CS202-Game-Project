#include "DamageManager.h"
void DamageManager::notify(const std::string& event, float value) {
    // Player attacking an enemy
    if (event == "PlayerAttack") {
        for (const auto& enemy : enemies) {
            if (player->attackRangeBox.intersects(enemy->boundingBox)) {
                enemy->takeDamage(value);
                std::cout << "Player hit an enemy for " << value << " damage!" << std::endl;
                player->hit = true;
            }
        }
    }
    else if (event == "GuardAttack") {
        for (const auto& enemy : enemies) {
            if (guard->attackRangeBox.intersects(enemy->boundingBox)) {
                enemy->takeDamage(value);
                std::cout << "Player hit an enemy for " << value << " damage!" << std::endl;
                guard->hit = true;
            }
        }
    }
    // Enemy attacking the player
    else if (event == "EnemyAttackPlayer") {
        player->takeDamage(value);
        std::cout << "Enemy hit the player for " << value << " damage!" << std::endl;
    }
    else if (event == "EnemyAttackGuard") {
        guard->takeDamage(value);
        std::cout << "Enemy hit the guard for " << value << " damage!" << std::endl;
    }
}
