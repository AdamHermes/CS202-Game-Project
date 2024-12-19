#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#ifndef MANAGER_H
#define MANAGER_H
using namespace std;
class Manager {
public:
    virtual void notify(const std::string& event, float value = 0) {
        std::cout << "FIRST NOTIF";
    }
    virtual void notify(float value, const sf::Vector2f& position) = 0;
    virtual ~Manager() = default;
};
#endif
