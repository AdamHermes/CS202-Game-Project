#include "Manager.h"
#ifndef GAMEENTITY_H
#define GAMEENTITY_H
class GameEntity {
protected:
    Manager* manager;

public:
    void setMediator(Manager* m) {
        manager = m;
    }

    virtual ~GameEntity() = default;
};
#endif
