#include "DamageManager.h"
void DamageManager::notify(const std::string& event, float value) {
    if (event == "PlayerAttack") {
        for (const auto& enemy : enemies) {
            if (player->attackRangeBox.intersects(enemy->boundingBox)) {
                value += player->baseStrength;
                enemy->takeDamage(value);
                std::cout << "Player hit an enemy for " << value << " damage!" << std::endl;
                player->hit = true;
                
                listener->notify(value, enemy->getSprite().getPosition());
            }
        }
    }
    else if (event == "GuardAttack") {
        for (const auto& enemy : enemies) {
            if (guard->attackRangeBox.intersects(enemy->boundingBox)) {
                value = 0.6 * value;
                enemy->takeDamage(value);
                std::cout << "Player hit an enemy for " << value << " damage!" << std::endl;
                guard->hit = true;
                listener->notify(value, enemy->getSprite().getPosition());
            }
        }
    }
    // Enemy attacking the player
    else if (event == "EnemyAttackPlayer") {
        if (player->getUsingSkill()) return;
        //value = 0.1 * value;
        player->takeDamage(value);
        std::cout << "Enemy hit the player for " << value << " damage!" << std::endl;
        listener->notify(value, player->getSprite().getPosition());
    }
    else if (event == "EnemyAttackGuard") {
        if (guard) {
            guard->takeDamage(value);
            std::cout << "Enemy hit the guard for " << value << " damage!" << std::endl;
        }
        
    }
}
