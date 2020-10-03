//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_KEYBOARDMOVEMENTSYSTEM_H
#define ECSGAME_KEYBOARDMOVEMENTSYSTEM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../ECS.h"
#include "../Components/Transform.h"
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
                    movement->moving.z -= 1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->moving.z += 1;
                }
            });
        } else if (event.key == GLFW_KEY_S) {
            myWorld->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->moving.z += 1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->moving.z -= 1;
                }
            });
        } else if (event.key == GLFW_KEY_A) {
            myWorld->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->moving.x -= 1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->moving.x += 1;
                }
            });
        } else if (event.key == GLFW_KEY_D) {
            myWorld->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->moving.x += 1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->moving.x -= 1;
                }
            });
        }
    }

    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Transform, Movement>(
                [&](Entity *ent, ComponentHandle<Transform> transform, ComponentHandle<Movement> movement) {
                    if (!movement->is_active) return;

                    transform->add_to_origin(transform->matrix * glm::vec4((glm::vec3(movement->moving) * movement->speed * deltaTime), 0.0));
                });
    }

    void unconfigure(World *pWorld) override {
        pWorld->unsubscribeAll(this);
    }

private:
    World *myWorld;
};

#endif //ECSGAME_KEYBOARDMOVEMENTSYSTEM_H
