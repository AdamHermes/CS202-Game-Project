#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"

class Gate {
private:
    sf::FloatRect boundingBox; // The area the player can interact with
public:
    Gate(float x, float y, float width, float height)
        : boundingBox(x, y, width, height) {}

    bool isPlayerEntering(const Character& player) const {
        return boundingBox.intersects(player.boundingBox);
    }

    sf::FloatRect getBoundingBox() const { return boundingBox; }
};
