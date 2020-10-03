#ifndef __PATHMOVEMENTSWITCHSYSTEM_H__
#define __PATHMOVEMENTSWITCHSYSTEM_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../ECS.h"
#include "../Components/PathMove.h"
#include "../Events/InputEvent.h"
#include "../Components/Movement.h" 
#include "../Components/MouseLook.h" 

using namespace ECS;

class PathMovementSwitchSystem : public EntitySystem, public EventSubscriber<InputEvent> {
    void configure(World *pWorld) override {
        myWorld = pWorld;

        myWorld->subscribe<InputEvent>(this);
    }

    void receive(World *pWorld, const InputEvent &event) override {
        if (event.key == GLFW_KEY_P) {
            myWorld->each<PathMove, Movement, MouseLook>([&](Entity *ent, ComponentHandle<PathMove> pathmove, ComponentHandle<Movement> movement, ComponentHandle<MouseLook> mouselook) {
                if (event.action == GLFW_PRESS) {
                    // Switch between them
                    if (pathmove->is_active) {
                        pathmove->is_active = false;
                        movement->is_active = true;
                        mouselook->is_active = true;
                    } else {
                        pathmove->is_active = true;
                        movement->is_active = false;
                        mouselook->is_active = false;
                    }
                }
            });
        }
    }

    void unconfigure(World *pWorld) override {
        pWorld->unsubscribeAll(this);
    }

private:
    World *myWorld;
};

#endif // __PATHMOVEMENTSWITCHSYSTEM_H__