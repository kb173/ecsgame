//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_GRAVITYSYSTEM_H
#define ECSGAME_GRAVITYSYSTEM_H

#include "../ECS.h"
#include "../Components/Transform.h"

using namespace ECS;

class GravitySystem : public EntitySystem {
public:
    explicit GravitySystem(float amount) {
        gravityAmount = amount;
    }

    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Transform>([&](Entity *ent, ComponentHandle<Transform> position) {
            position->translate(glm::vec3(0.0f, gravityAmount * deltaTime, 0.0f));
        });
    }

private:
    float gravityAmount;
};

#endif //ECSGAME_GRAVITYSYSTEM_H
