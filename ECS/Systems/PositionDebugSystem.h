//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_POSITIONDEBUGSYSTEM_H
#define ECSGAME_POSITIONDEBUGSYSTEM_H

#include <iostream>

#include "../ECS.h"
#include "../Components/Position.h"

using namespace ECS;

class PositionDebugOutputSystem : public EntitySystem {
public:
    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Position>([&](Entity *ent, ComponentHandle<Position> position) {
            std::cout << ent->getEntityId() << ": "
                      << position->x << ", "
                      << position->y << ", "
                      << position->z
                      << std::endl;
        });
    }
};

#endif //ECSGAME_POSITIONDEBUGSYSTEM_H
