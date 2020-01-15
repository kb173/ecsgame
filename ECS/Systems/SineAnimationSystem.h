//
// Created by karl on 15.01.20.
//

#ifndef ECSGAME_SINEANIMATIONSYSTEM_H
#define ECSGAME_SINEANIMATIONSYSTEM_H

#include "../ECS.h"
#include "../Components/Transform.h"
#include "../Components/SineAnimation.h"

using namespace ECS;

class SineAnimationSystem : public EntitySystem {
public:
    void tick(World *pWorld, float deltaTime) override {
        passedTime += deltaTime;

        pWorld->each<Transform, SineAnimation>([&](Entity *ent, ComponentHandle<Transform> transform, ComponentHandle<SineAnimation> anim) {
            transform->translate(anim->maxDistance * glm::sin(passedTime * anim->speedScale) * deltaTime);
        });
    }

private:
    float passedTime = 0.0;
};

#endif //ECSGAME_SINEANIMATIONSYSTEM_H
