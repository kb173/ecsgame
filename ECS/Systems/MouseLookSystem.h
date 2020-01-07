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
            double xOffset = event.newX - lastX;
            double yOffset = lastY - event.newY;

            lastX = event.newX;
            lastY = event.newY;

            yaw += xOffset * mouse->sensitivity;
            pitch += yOffset * mouse->sensitivity;
        });
    }

    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Transform, MouseLook, Camera>([&](Entity *ent, ComponentHandle<Transform> transform, ComponentHandle<MouseLook> mouse, ComponentHandle<Camera> camera) {
            if(pitch > 89.0f)
                pitch =  89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;

            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

            glm::vec3 cameraOrigin = transform->getPosition();
            glm::vec3 cameraFront = glm::normalize(direction);
            glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
            glm::vec3 cameraRight = glm::cross(cameraFront, cameraUp);

            glm::mat4x4 lookAt = glm::lookAt(cameraOrigin, cameraOrigin + cameraFront, cameraUp);

            lookAt[3] = glm::vec4(cameraOrigin, 1.0);

            transform->matrix = lookAt;
        });
    }

private:
    double lastX;
    double lastY;

    double pitch = 0.0;
    double yaw = 0.0;

    World *myWorld;
};

#endif //ECSGAME_MOUSELOOKSYSTEM_H
