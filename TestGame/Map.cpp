#include "Map.h"
#include <iostream>
#include <SFML/Graphics.hpp>

bool Map::loadTexture(std::string filename) {
    if (mapTexture.loadFromFile(filename)) cout << "Load Success";
    else return false;
    mapSprite.setTexture(mapTexture);
    mapSprite.setTextureRect(sf::IntRect(0, 0, 3240, 3240));
    return true;
}

//void Map::addObstacles(std::shared_ptr<Obstacle> obstacle) {
//    obstacles.push_back(std::move(obstacle));
//}


bool Map::loadFromTMX(const std::string& fileName) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load .tmx file: " << fileName << std::endl;
        return false;
    }

    // Find the <map> element
    tinyxml2::XMLElement* mapElement = doc.FirstChildElement("map");
    if (!mapElement) {
        std::cerr << "No <map> element found in the TMX file." << std::endl;
        return false;
    }

    // Find the "objectgroup" element with the name "Walls"
    for (tinyxml2::XMLElement* objectGroupElement = mapElement->FirstChildElement("objectgroup");
        objectGroupElement != nullptr;
        objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup")) {

        // Check if the name of the objectgroup is "Walls"
        const char* name = objectGroupElement->Attribute("name");
        if (name && std::string(name) == "Walls") {

            // Iterate through all <object> elements within this objectgroup
            for (tinyxml2::XMLElement* objectElement = objectGroupElement->FirstChildElement("object");
                objectElement != nullptr;
                objectElement = objectElement->NextSiblingElement("object")) {

                // Extract object attributes (x, y, width, height)
                int x = static_cast<int>(objectElement->FloatAttribute("x"));
                int y = static_cast<int>(objectElement->FloatAttribute("y"));
                int width = static_cast<int>(objectElement->FloatAttribute("width"));
                int height = static_cast<int>(objectElement->FloatAttribute("height"));

                // Create and add an obstacle
                obstacles.push_back(make_shared<Obstacle>(x, y, width, height));

            }
        }
    }
    cout << obstacles.size() << endl;
    return true;
}
bool Map::checkCollision(int px, int py, int pwidth, int pheight) const {
    for (const auto& obstacle : obstacles) {
        if (obstacle->collides(px, py, pwidth, pheight)) {
            return true;
        }
    }
    return false;
}
void Map::drawWalls(sf::RenderWindow& window) const {
    for (const auto& obstacle : obstacles) {
        // Create a rectangle to represent the obstacle (wall)
        sf::RectangleShape wallShape;
        wallShape.setPosition(sf::Vector2f(obstacle->getX(), obstacle->getY()));
        wallShape.setSize(sf::Vector2f(obstacle->getWidth(), obstacle->getHeight()));
        wallShape.setFillColor(sf::Color::Transparent);  // Transparent fill
        wallShape.setOutlineColor(sf::Color::Red);       // Red outline for visibility
        wallShape.setOutlineThickness(1.0f);             // Thin outline

        // Draw the wall boundary
        window.draw(wallShape);
    }
}
void Map::drawTo(sf::RenderWindow& window) {
    window.draw(mapSprite);
}