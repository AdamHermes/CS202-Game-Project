#pragma once
#include "Map.h"
class Door : public Obstacle {
private:
    int direction;
public:
    Door(int x, int y, int width, int height, int dir)
        : Obstacle(x, y, width, height), direction(dir) {}
    int getDir() const { return direction; }
    
};
