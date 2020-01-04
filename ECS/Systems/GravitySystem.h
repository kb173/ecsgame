//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_GRAVITYSYSTEM_H
#define ECSGAME_GRAVITYSYSTEM_H

#include "../ECS.h"
#include "../Components/Position.h"

using namespace ECS;

class GravitySystem : public EntitySystem {
public:
    explicit GravitySystem(float amount) {
        gravityAmount = amount;
    }

    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Position>([&](Entity *ent, ComponentHandle<Position> position) {
            position->y += gravityAmount * deltaTime;
        });
    }

private:
    float gravityAmount;
};

#endif //ECSGAME_GRAVITYSYSTEM_H
