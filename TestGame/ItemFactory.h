#pragma once

#include "Items.h"
class ItemFactory {
public:
    static std::shared_ptr<Items> createItems(ItemType type, sf::Vector2f spawnLocation,string text = "Hello Nice to meet you") {
        if (type == ItemType::health) {
            return std::make_shared<Items>(type, "../Assets/Character/Items/health_portion.png",spawnLocation);
        }
        else if (type == ItemType::speed) {
            return std::make_shared<Items>(type, "../Assets/Character/Items/speed_portion.png", spawnLocation);
        }
        else if (type == ItemType::power) {
            return std::make_shared<Items>(type, "../Assets/Character/Items/power_portion.png",spawnLocation);
        }
        else if (type == ItemType::statue) {
            return std::make_shared<Items>(type,"../Assets/Character/Items/soul.png", spawnLocation - sf::Vector2f(0,32) , text, "../Assets/Character/Dialog/dialog.png", "../Assets/Roboto-Black.tTf");
        }
        return nullptr;
    }
};
