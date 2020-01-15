//
// Created by karl on 07.01.20.
//

#ifndef ECSGAME_MOUSELOOKSYSTEM_H
#define ECSGAME_MOUSELOOKSYSTEM_H

#include "../ECS.h"
#include "../Components/Transform.h"
#include "../Components/MouseLook.h"
#include "../Events/MouseMoveEvent.h"

using namespace ECS;

class MouseLookSystem : public EntitySystem, public EventSubscriber<MouseMoveEvent>  {
public:
    explicit MouseLookSystem(int width, int height) : lastX(width / 2.0), lastY(height / 2.0) {}

    void configure(World *pWorld) override {
        myWorld = pWorld;

        myWorld->subscribe<MouseMoveEvent>(this);
    }

    void unconfigure(World *pWorld) override {
        pWorld->unsubscribeAll(this);
    }

    void receive(World *pWorld, const MouseMoveEvent &event) override {
        pWorld->each<Transform, MouseLook, Camera>([&](Entity *ent, ComponentHandle<Transform> transform, ComponentHandle<MouseLook> mouse, ComponentHandle<Camera> camera) {
            double xOffset = lastX - event.newX;
            double yOffset = lastY - event.newY;

            lastX = event.newX;
            lastY = event.newY;

            mouse->yaw += xOffset * mouse->sensitivity;
            mouse->pitch += yOffset * mouse->sensitivity;
        });
    }

    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Transform, MouseLook, Camera>([&](Entity *ent, ComponentHandle<Transform> transform, ComponentHandle<MouseLook> mouse, ComponentHandle<Camera> camera) {
            if(mouse->pitch > 50.0f)
                mouse->pitch =  50.0f;
            if(mouse->pitch < -50.0f)
                mouse->pitch = -50.0f;

            glm::mat4x4 newTransform = glm::mat4x4(1.0);

            newTransform = glm::rotate(newTransform, glm::radians((float)mouse->pitch), transform->right());
            newTransform = glm::rotate(newTransform, glm::radians((float)mouse->yaw), glm::vec3(0.0, 1.0, 0.0));

            newTransform[3] = transform->matrix[3];

            transform->matrix = newTransform;
        });
    }

private:
    double lastX;
    double lastY;

    World *myWorld;
};

#endif //ECSGAME_MOUSELOOKSYSTEM_H
