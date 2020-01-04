//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_KEYBOARDMOVEMENTSYSTEM_H
#define ECSGAME_KEYBOARDMOVEMENTSYSTEM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../ECS.h"
#include "../Components/Position.h"
#include "../Events/InputEvent.h"
#include "../Components/Movement.h" 

using namespace ECS;

class KeyboardMovementSystem : public EntitySystem, public EventSubscriber<InputEvent> {
    void configure(World *pWorld) override {
        myWorld = pWorld;

        myWorld->subscribe<InputEvent>(this);
    }

    void receive(World *pWorld, const InputEvent &event) override {
        if (event.key == GLFW_KEY_W) {
            myWorld->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->movingZ = -1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->movingZ = 0;
                }
            });
        } else if (event.key == GLFW_KEY_S) {
            myWorld->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->movingZ = 1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->movingZ = 0;
                }
            });
        } else if (event.key == GLFW_KEY_A) {
            myWorld->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->movingX = 1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->movingX = 0;
                }
            });
        } else if (event.key == GLFW_KEY_D) {
            myWorld->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->movingX = -1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->movingX = 0;
                }
            });
        }
        std::cout << "MyEvent was emitted!" << std::endl;
    }

    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Position, Movement>(
                [&](Entity *ent, ComponentHandle<Position> position, ComponentHandle<Movement> movement) {
                    position->x += movement->movingX * movement->speedX * deltaTime;
                    position->y += movement->movingY * movement->speedY * deltaTime;
                    position->z += movement->movingZ * movement->speedZ * deltaTime;
                });
    }

    void unconfigure(World *pWorld) override {
        pWorld->unsubscribeAll(this);
    }

private:
    World *myWorld;
};

#endif //ECSGAME_KEYBOARDMOVEMENTSYSTEM_H
