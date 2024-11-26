#include "tinyxml2.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"
#include "Character.h"
#include "Camera.h"
#include "Enemy.h"
using namespace std;


int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1280), "RPG GAME");
    Character player;    
    Map gameMap;
    if (!gameMap.loadTexture("../Assets/Character/Textures/map0.png")) {
        return -1; // Exit if texture fails to load
    }
    if (!gameMap.loadFromTMX("../Assets/Character/TMX MAP/map0.tmx")) {
        return -1;
    }
    /*if (!gameMap.loadTileAndObjectLayers("../Assets/Character/TMX MAP/draft.tmx")) {
        return -1;
    }*/
    player.loadTexture("../Assets/Character/Textures/characters.png", 340, 960 );
    player.updateBoundingBox();
    int num = 2;
    Camera camera(720, 480);
    //Enemy enemy(340, 760, 100.0f, 0.1f, "../Assets/Character/Test/warrior1.png");
    //Enemy enemy2(240, 760, 100.0f, 0.1f, "../Assets/Character/mon1.png");
    // Optionally set a zoom level (1.5 zooms out slightly, showing more of the world)
    camera.setZoom(1.5f);
    Enemy enemy;
    enemy.loadTexture("../Assets/Character/Textures/demon1.png", 240, 960);
    // Set up the game world size, e.g., 2000x2000 pixels
    sf::FloatRect worldBounds(0, 0, 2000, 2000);
    int num2 = 1;
    camera.setWorldBounds(worldBounds);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        static sf::Clock deltaClock;
        float deltaTime = deltaClock.restart().asSeconds();
        sf::Vector2f playerPosition = player.getSprite().getPosition();
        //enemy.update(deltaTime);
        // Update camera based on player position
        camera.update(playerPosition);
        
        // Clear, apply camera view, draw everything, display
        window.clear();
        camera.applyView(window);
        
        bool isMoving = false;
        bool isMoving1 = false;
        enemy.handleMovement(gameMap, num2, isMoving1);
        player.handleMovement(gameMap, enemy, num, isMoving);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            player.fightBow(num);
            isMoving = true;
        }
        //if (!isMoving) {
        //    // Reset animation if no key is pressed
        //    player.resetAnimation();
        //}
        window.clear();
        gameMap.drawTo(window);
        //gameMap.drawWalls(window);
        player.drawTo(window);
        enemy.drawTo(window);
        /*window.draw(wall);
        window.draw(wall1);
        window.draw(wall2);
        window.draw(block);*/
        //window.draw(enemy.getSprite()); // Draw the enemy
        //window.draw(enemy2.getSprite());
        window.display();
    }

    return 0;
}
