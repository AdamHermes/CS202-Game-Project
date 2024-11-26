#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"
enum class EnemyType {
    Goblin,
    Demon,
    Golem
};
class Enemy {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    bool isHiden = false;
    int totalFrames = 0;
    int currentFrame = 0;
    float frameDuration = 0.1f;
    sf::Clock animationClock;
    EnemyType enemyType;
    enum Direction {
        Up = 2,
        Down = 1,
        Left = 4,
        Right = 3
    };

public:
    sf::FloatRect boundingBox;
    float offsetX = (64 - 32) / 2;
    float offsetY = 0;
    Enemy(EnemyType type) {
        enemyType = type;
        switch (type) {
        case EnemyType::Goblin:
            totalFrames = 11;
            loadTexture("../Assets/Character/Enemies/goblin.png", 240, 960);
        case EnemyType::Demon:
            totalFrames = 4;
            loadTexture("../Assets/Character/Enemies/demon1.png", 440, 960);
        case EnemyType::Golem:
            totalFrames = 7;
            loadTexture("../Assets/Character/Enemies/golem.png", 440, 960);
        }
    }
    void updateBoundingBox() {
        sf::Vector2f position = sprite.getPosition();
        boundingBox = sf::FloatRect(position.x + offsetX, position.y + offsetY, 32, 64);
    }
    bool checkCollision(const sf::FloatRect& otherBox) const {
        return boundingBox.intersects(otherBox);
    }
    void changePos(int direction);
    void loadTexture(std::string filename, float x, float y);
    void drawTo(sf::RenderWindow& window) const;
    void handleMovement(Map& gameMap,int& direction, bool& isMoving);
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
