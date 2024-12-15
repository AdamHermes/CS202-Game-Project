//// GameLoop.cpp
//#include "GameLoop.h"
//
//GameLoop::GameLoop() : window(sf::VideoMode(1500, 1000), "RPG GAME"), game() {
//    // Initialize game components here if needed
//}
//
//void GameLoop::run() {
//    sf::Clock clock;
//    while (window.isOpen()) {
//        float deltaTime = clock.restart().asSeconds();
//
//        handleEvents();
//        update();
//        render();
//    }
//}
//
//void GameLoop::handleEvents() {
//    sf::Event event;
//    while (window.pollEvent(event)) {
//        if (event.type == sf::Event::Closed)
//            window.close();
//    }
//}
//
//void GameLoop::update() {
//    game.update();
//    // Update other entities or game states if necessary
//}
//
//void GameLoop::render() {
//    window.clear();
//    game.render(window); // Assuming Game has a render method
//    window.display();
//}
