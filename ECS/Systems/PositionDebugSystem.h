//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_POSITIONDEBUGSYSTEM_H
#define ECSGAME_POSITIONDEBUGSYSTEM_H

#include <iostream>

#include "../ECS.h"
#include "../Components/Transform.h"

using namespace ECS;

class PositionDebugOutputSystem : public EntitySystem {
public:
    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Transform>([&](Entity *ent, ComponentHandle<Transform> transform) {
            std::cout << ent->getEntityId() << ": "
                      << transform->getPosition().x << ", "
                      << transform->getPosition().y << ", "
                      << transform->getPosition().z
                      << std::endl;
        });
    }
};

#endif //ECSGAME_POSITIONDEBUGSYSTEM_H
