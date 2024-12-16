#pragma once

#include "Items.h"
class ItemFactory {
public:
    static std::shared_ptr<Items> createItems(ItemType type, sf::Vector2f spawnLocation) {
        if (type == ItemType::health) {
            return std::make_shared<Items>(type, "../Assets/Character/Items/health_portion.png",spawnLocation);
        }
        else if (type == ItemType::speed) {
            return std::make_shared<Items>(type, "../Assets/Character/Items/speed_portion.png", spawnLocation);
        }
        return nullptr;
    }
};
