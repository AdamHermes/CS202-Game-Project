#include "DamageManager.h"
void DamageManager::notify(const std::string& event, float value) {
    // Player attacking an enemy
    if (event == "PlayerAttack") {
        for (const auto& enemy : enemies) {
            if (player.attackRangeBox.intersects(enemy->boundingBox)) {
                enemy->takeDamage(value);
                std::cout << "Player hit an enemy for " << value << " damage!" << std::endl;
            }
        }
    }

    // Enemy attacking the player
    else if (event == "EnemyAttack") {
        player.takeDamage(value);
        std::cout << "Enemy hit the player for " << value << " damage!" << std::endl;
        
    }
}
