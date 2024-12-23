#include "GameState.h"

GameState::GameState(Callback gameOverCallback, Callback gameWinCallback)
    : camera(720, 480), isFighting(false), targetLocation(640,600){
    // Initialize player
    player = std::make_shared<Character>(
        "../Assets/Character/Textures/characters.png",
        "../Assets/Character/Textures/health.png",
        CharacterType::player
    );
    guard = std::make_shared<Character>(
        "../Assets/Character/Textures/character1.png",
        "../Assets/Character/Textures/health.png",
        CharacterType::guard
    );
    // Initialize game map
    gameMap = std::make_shared<Map>();
    if (!gameMap->loadTexture("../Assets/Character/Textures/map0.png")) {
        throw std::runtime_error("Failed to load map texture.");
    }
    if (!gameMap->loadFromTMX("../Assets/Character/TMX MAP/map0.tmx")) {
        throw std::runtime_error("Failed to load map data.");
    }

    // Set up player
    player->updateBoundingBox();
    player->equipWeapon(WeaponType::Sword);
    player->equipWeapon(WeaponType::Bow);
    player->equipWeapon(WeaponType::None);
    auto start_weapon = player->getWeapon(0);
    player->setCurWeapon(start_weapon);
    

    // Set up camera
    camera.setZoom(1.5f);
    sf::FloatRect worldBounds(0, 0, 2560, 2560);
    camera.setWorldBounds(worldBounds);
   
    gameLoop = std::make_unique<GameLoop>(player, gameMap,guard);
    gameLoop->loadMusic("../Assets/SoundTrack/level1.mp3",false);
    gameLoop->playMusic();
    gameLoop->addLevels();
    gameLoop->loadEnemiesForRooms(0);
    gameLoop->loadItemsForRooms(0);
    gameLoop->initializeTextDamage();
    gameLoop->updateDamageManager();

    gameLoop->doorPositions.push_back({
            std::make_tuple(256, 1088, 96, 32,0), // Door In for Room 1
            std::make_tuple(512, 928, 32, 64,0)   // Door Out for Room 1
        });
    
    gameLoop->doorPositions.push_back({
            std::make_tuple(768, 480, 64, 32,0), // Door In for Room 1
            std::make_tuple(1056, 320, 32, 96,3)   // Door Out for Room 1
        });
    gameLoop->doorPositions.push_back({
            std::make_tuple(1344,640,64,32,2),
            std::make_tuple(1120,896,32,64,1)
        });
    level = gameLoop->getLevel(0);
    level->loadDoors(gameLoop->doorPositions);
    room = level->getRoom(0);
    
    float locationTolerance = 10.0f;
    this->gameOverCallback = gameOverCallback;
    this->gameWinCallback = gameWinCallback;
    
}

void GameState::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (!player->isDead()) {
        static sf::Clock deltaClock;
        float deltaTime = deltaClock.restart().asSeconds();
        sf::Vector2f playerPosition = player->getSprite().getPosition();

        if (!isFighting) {
            camera.update(playerPosition,!player->isDead());
        }

        window.clear();
        camera.applyView(window);
        static bool finale = false;
        if (gameLoop->getCurLevelIndex() == 3) {
            finale = true;
        }
        bool isMoving = false;
        bool isMoving1 = false;

        static bool wasJPressed = false;
       /* if (gameLoop->getCurLevelIndex() == 2) {
            
        }*/
       if ((gameLoop->getCurLevelIndex() == 2 || gameLoop->getCurLevelIndex() == 1) && guard) {


            guard->handleGuardianMovement(gameMap, player, room->getEnemies());
       }
       static bool wasKeyEPressed = false; // Tracks if the E key was previously pressed
       static bool wasKeyOPressed = false;
       static bool isDialogOpen = false;  // Tracks whether a dialog is currently open
       static shared_ptr<Items> currentStatue = nullptr; // Tracks the statue currently showing a dialog
       
       if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
           if (!wasKeyEPressed) { // Only trigger when the key is first pressed
               wasKeyEPressed = true;

               auto item = player->checkItemNearby(level->getItems());
               if (item) {
                   if (item->getType() == ItemType::statue) {

                        if (isDialogOpen && currentStatue == item) {
                            // Hide the dialog if the same statue is interacted with
                            item->hideDialog();
                            isDialogOpen = false;
                            currentStatue = nullptr;
                        }
                        else {
                            // Show the dialog for the statue
                            if (currentStatue && isDialogOpen) {
                                // Hide the currently open dialog if switching statues
                                currentStatue->hideDialog();
                            }
                            item->showDialog();
                            isDialogOpen = true;
                            currentStatue = item;
                        }
                    }
                   
                   else {
                       player->takePortions(item);
                   }
               }
           }
       }
       else {
           // Reset the key press state when the key is released
           wasKeyEPressed = false;
       }

        if (!finale) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
                if (!wasKeyOPressed && !used) {
                    player->loadSkill();
                    player->skillDuration.restart();  // Start the 6 seconds countdown
                    wasKeyOPressed = true;
                    used = true;

                }
                else {
                    wasKeyOPressed = false;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                auto usedItem = player->getItems(); // Use the getter
                if (usedItem[0]) { // Check if the first item exists
                    player->applyItemEffect(usedItem[0]);
                    player->setUsedItem(0);
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                auto usedItem = player->getItems();
                if (usedItem[1]) {
                    player->applyItemEffect(usedItem[1]);
                    player->setUsedItem(1);
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
                auto usedItem = player->getItems();
                if (usedItem[2]) {
                    player->applyItemEffect(usedItem[2]);
                    player->setUsedItem(2);
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                isFighting = true;
                isMoving = false;
                wasJPressed = false;
                player->updateState(isFighting, num, WeaponType::Sword);
                player->setCurWeapon(player->getWeapon(0));
                player->setShooting(false);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
                isFighting = true;
                isMoving = false;
                wasJPressed = true;
                player->updateState(isFighting, num, WeaponType::Bow);
                player->setCurWeapon(player->getWeapon(1));
                player->setShooting(true);
            }
            else {
                if (wasJPressed) {
                    player->setShooting(false); // Clear arrows when spacebar is released
                    wasJPressed = false;
                }
                isFighting = false;
                player->updateState(isFighting, num, WeaponType::None);
                player->setCurWeapon(player->getWeapon(2));
            }
            if (player->getUsingSkill()) {
                player->fightSkill();
            }
        }
        
        
        
        if (isFighting) {

            if (wasJPressed) {
                player->fightBow(num, CharacterType::player, room->getEnemies(), gameMap);

            }
            else {             
                player->fightSword(num, room->getEnemies());
            }
        }
        else {
            player->handleMovement(gameMap, room->getEnemies(), num, isMoving);
        }
        
        player->updateSpriteHealth(camera);
        player->updateItemPositions(camera.getView());
        if (guard) {

            guard->updateSpriteHealth(camera);
        }

    }
    
}

void GameState::update() {
    
    if (gameLoop->getCurLevelIndex() < 3) {
        if (player->isDead()) {

            gameOverCallback();

            return;
        }
        if (guard && gameLoop->isGuardDead()) {

            guard.reset();
        }
        if (!gameLoop->playerInRoom && gameLoop->updateDoors()) {
            gameLoop->playerInRoom = true;
            int roomIndex = gameLoop->getCurLevel()->getRoomIndex();
            gameLoop->getCurLevel()->setRoomIndex(1);
            room = gameLoop->getCurLevel()->getRoom(roomIndex);
        }
        if (room->isCleared() && gameLoop->playerInRoom) {
            gameLoop->playerInRoom = false;
        }
        gameLoop->updateLevel2();
        if (gameLoop->update()) {
            used = false;
            if (gameLoop->getCurLevelIndex() < 3) {
                room = gameLoop->getCurLevel()->getRoom(0);
                level = gameLoop->getCurLevel();
            }
        }


    }
    else if (gameLoop->getCurLevelIndex() == 3) {
        level = gameLoop->getCurLevel();
        if (std::abs(player->getSprite().getPosition().x - targetLocation.x) <= locationTolerance &&
            std::abs(player->getSprite().getPosition().y - targetLocation.y) <= locationTolerance) {

            if (!playerAtLocation) {
                stayClock.restart();  // Restart the clock when the player enters the target location
                playerAtLocation = true;
            }

            // Check if the player has stayed at the location for the required time
            if (stayClock.getElapsedTime().asSeconds() >= requiredTimeAtLocation) {

                gameWinCallback();
            }
        }
        else {
            // Reset the clock if the player moves away from the location
            playerAtLocation = false;
        }
    }
    
    
}

void GameState::draw(sf::RenderWindow& window) {
    
    gameMap->drawTo(window);

    if (gameLoop->getCurLevelIndex() != 3) {
        gameLoop->render(window);
    }
    else if (gameLoop->getCurLevelIndex() == 3) gameLoop->getCurLevel()->renderItems(window);
    if (player) {
        player->drawTo(window);
    }
    
    if ((gameLoop->getCurLevelIndex() == 2 || gameLoop->getCurLevelIndex() == 1) && guard) {
        guard->drawTo(window);
;
    }
    //gameMap->drawWalls(window);
    //sf::RectangleShape rangeShape(sf::Vector2f(guard->boundingBox.width, guard->attackRangeBox.height));
    //rangeShape.setPosition(guard->attackRangeBox.left, guard->attackRangeBox.top);
    //rangeShape.setFillColor(sf::Color(255, 0, 0, 100)); // Semi-transparent red

    //// Draw the range shape on the window for debugging
    //window.draw(rangeShape);

}
