#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"
class Enemy {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    bool isHiden = false;
    int currentFrame = 0;
    float frameDuration = 0.1f;
    sf::Clock animationClock;

    enum Direction {
        Up = 2,
        Down = 1,
        Left = 4,
        Right = 3
    };

public:
    sf::FloatRect boundingBox;
    float offsetX = (64 - 28) / 2;
    float offsetY = (64 - 48);
    void updateBoundingBox() {
        sf::Vector2f position = sprite.getPosition();
        boundingBox = sf::FloatRect(position.x + offsetX, position.y + offsetY, 28, 48);
    }
    bool checkCollision(const sf::FloatRect& otherBox) const {
        return boundingBox.intersects(otherBox);
    }
    void changePos(int direction);
    void loadTexture(std::string filename, float x, float y);
    void drawTo(sf::RenderWindow& window) const;
    void handleMovement(Map& gameMap,int& direction, bool& isMoving);
};
