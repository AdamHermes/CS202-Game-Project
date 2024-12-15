#include "GameState.h"

GameState::GameState()
    : camera(720, 480), isFighting(false) {
    // Initialize player
    player = std::make_shared<Character>(
        "../Assets/Character/Textures/character1.png",
        "../Assets/Character/Textures/health.png"
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
    player->equipWeapon(WeaponType::Spear);
    player->equipWeapon(WeaponType::Bow);
    player->equipWeapon(WeaponType::None);
    auto start_weapon = player->getWeapon(0);
    player->setCurWeapon(start_weapon);

    // Set up camera
    camera.setZoom(1.5f);
    sf::FloatRect worldBounds(0, 0, 2000, 2000);
    camera.setWorldBounds(worldBounds);
    
    gameLoop = std::make_unique<GameLoop>(player, gameMap);
    gameLoop->addLevels();
    gameLoop->loadEnemiesForRooms(0);
    gameLoop->updateDamageManager();
    level = gameLoop->getLevel(0);
    room = level->getRoom(0);
    
}

void GameState::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    static sf::Clock deltaClock;
    float deltaTime = deltaClock.restart().asSeconds();
    sf::Vector2f playerPosition = player->getSprite().getPosition();

    if (!isFighting) {
        camera.update(playerPosition);
    }

    window.clear();
    camera.applyView(window);

    bool isMoving = false;
    bool isMoving1 = false;


    static bool wasJPressed = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        player->takePortions();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        isFighting = true;
        isMoving = false;
        wasJPressed = false;
        player->updateState(isFighting, num, WeaponType::Spear);
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


    if (isFighting) {
        if (wasJPressed) {
            player->fightBow(num, room->getEnemies(), gameMap);

        }
        else {
            player->fightSpear(num, room->getEnemies());
        }
    }
    else {
        player->handleMovement(gameMap, room->getEnemies(), num, isMoving);
    }
    player->updateSpriteHealth(camera);
}

void GameState::update() {
    if (gameLoop->update()) {
        room = gameLoop->getCurLevel()->getRoom(0);
    }
}

void GameState::draw(sf::RenderWindow& window) {
    // Draw game map and player
    gameMap->drawTo(window);
    gameLoop->render(window);
    if (player) {
        player->drawTo(window);
    }
}
