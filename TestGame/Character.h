#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"
#include "Enemy.h"
using namespace std;
class Character {
private:
    double health, mana;

    bool isMoving = false;
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
    float offsetX = (64 - 33) / 2;
    float offsetY = (64 - 52);
    sf::Sprite getSprite() {
        return sprite;
    }
    void updateBoundingBox() {
        sf::Vector2f position = sprite.getPosition();
        boundingBox = sf::FloatRect(position.x + offsetX, position.y + offsetY, 33, 52);
    }
    

    void changePos(int direction);
    void resetAnimation() {
        currentFrame = 0;
    }
    void handleMovement(Map& gameMap, Enemy& enemy, int& num, bool& isMoving);
    void fight(int direction);
    void fightBow(int direction);
    void loadTexture(std::string filename, float x, float y);
    void drawTo(sf::RenderWindow& window) const;
    void hide() {
        isHiden = true;
    }
    void reveal() {
        isHiden = false;
    }
    void drawBoundingBox(sf::RenderWindow& window) {
        sf::RectangleShape boundingBoxShape;
        boundingBoxShape.setSize(sf::Vector2f(boundingBox.width, boundingBox.height));
        boundingBoxShape.setPosition(boundingBox.left, boundingBox.top);
        boundingBoxShape.setFillColor(sf::Color::Transparent);  // Transparent fill
        boundingBoxShape.setOutlineColor(sf::Color::Red);  // Red outline for visibility
        boundingBoxShape.setOutlineThickness(2);  // Thickness of the outline

        window.draw(boundingBoxShape);  // Draw the bounding box on the window
    }
};
