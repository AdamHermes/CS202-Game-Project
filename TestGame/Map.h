#pragma once
#include <iostream>
#include "tinyxml2.h"
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;
class Obstacle {
protected:
    int x, y, width, height;
public:
    Obstacle() = default;
    Obstacle(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}

    bool collides(int px, int py, int pwidth, int pheight) const {
        return (px < x + width && px + pwidth > x &&
            py < y + height && py + pheight > y);
    }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};
class Map : public Obstacle {
public:
    
    sf::Sprite mapSprite;
    sf::Texture mapTexture;
    vector<unique_ptr <Obstacle>> obstacles;
    bool loadTexture(std::string filename);
    void addObstacles(std::unique_ptr<Obstacle> obstacle);
    bool loadFromTMX(const std::string& fileName);
    bool checkCollision(int px, int py, int pwidth, int pheight) const;
    void drawWalls(sf::RenderWindow& window) const;
    void drawTo(sf::RenderWindow& window);
};