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
    int num = 2;
    player.loadTexture("../Assets/Character/Textures/characters.png",false, num,740, 1000 );
    player.updateBoundingBox();
    player.equipWeapon(WeaponType::Sword);
    Camera camera(720, 480);
    //Enemy enemy(340, 760, 100.0f, 0.1f, "../Assets/Character/Test/warrior1.png");
    //Enemy enemy2(240, 760, 100.0f, 0.1f, "../Assets/Character/mon1.png");
    // Optionally set a zoom level (1.5 zooms out slightly, showing more of the world)
    camera.setZoom(1.5f);
    //Enemy goblin(EnemyType::Goblin);
    Enemy golem(EnemyType::Golem);
    // Set up the game world size, e.g., 2000x2000 pixels
    sf::FloatRect worldBounds(0, 0, 2000, 2000);
    int num2 = 1;
    camera.setWorldBounds(worldBounds);
    bool isFighting = false;
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
        if (!isFighting) {
            camera.update(playerPosition);
        }

        
        // Clear, apply camera view, draw everything, display
        window.clear();
        camera.applyView(window);
        
        bool isMoving = false;
        bool isMoving1 = false;
        golem.handleMovement(gameMap, player);

 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            isFighting = true;
            isMoving = false;
        }
        else {
            isFighting = false;
        }
        player.updateState(isFighting,num);

        if (isFighting) {
            player.fightSword(num, golem);
        }
        else {
            player.handleMovement(gameMap, golem, num, isMoving);
        }
        //if (!isMoving) {
        //    // Reset animation if no key is pressed   
        //    player.resetAnimation();
        //}
        window.clear();
        gameMap.drawTo(window);
        //gameMap.drawWalls(window);

        golem.drawTo(window);
        player.drawTo(window);
        golem.drawBoundingBox(window);
        player.drawBoundingBox(window);
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
