#ifndef __PATHMOVESYSTEM_H__
#define __PATHMOVESYSTEM_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../ECS.h"
#include "../Components/Transform.h"
#include "../Events/InputEvent.h"
#include "../Components/PathMove.h"

using namespace ECS;

// Spline helper functions

// Calculate the t value for a Catmull–Rom spline
float get_t(float alpha, float t, glm::vec3 p0, glm::vec3 p1) {
    float a = pow((p1.x - p0.x), 2.0f) + pow((p1.y - p0.y), 2.0f) + pow((p1.z - p0.z), 2.0f);
    float b = pow(a, alpha * 0.5f);
    
    return (b + t);
}

// Given four points, calculate an interpolated point between p1 and p2 using a Catmul-Rom spline.
// t specifies the position along the path, with t=0 being p1 and t=2 being p2.
glm::vec3 catmul(float alpha, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t) {
    float t0 = 0.0f;
    float t1 = get_t(alpha, t0, p0, p1);
    float t2 = get_t(alpha, t1, p1, p2);
    float t3 = get_t(alpha, t2, p2, p3);

    // Lerp t to be between t1 and t2
    t = t1 + t * (t2 - t1);

    glm::vec3 A1 = (t1-t)/(t1-t0)*p0 + (t-t0)/(t1-t0)*p1;
    glm::vec3 A2 = (t2-t)/(t2-t1)*p1 + (t-t1)/(t2-t1)*p2;
    glm::vec3 A3 = (t3-t)/(t3-t2)*p2 + (t-t2)/(t3-t2)*p3;
    
    glm::vec3 B1 = (t2-t)/(t2-t0)*A1 + (t-t0)/(t2-t0)*A2;
    glm::vec3 B2 = (t3-t)/(t3-t1)*A2 + (t-t1)/(t3-t1)*A3;
    
    glm::vec3 C = (t2-t)/(t2-t1)*B1 + (t-t1)/(t2-t1)*B2;

    return C;
}

class PathMoveSystem : public EntitySystem, public EventSubscriber<InputEvent> {
    void configure(World *pWorld) override {
        myWorld = pWorld;

        // TODO: Changing velocity
        myWorld->subscribe<InputEvent>(this);
    }

    void receive(World *pWorld, const InputEvent &event) override {
        if (event.key == GLFW_KEY_W) {
            myWorld->each<PathMove>([&](Entity *ent, ComponentHandle<PathMove> pathmove) {
                if (event.action == GLFW_PRESS) {
                    // TODO: Velocity adder
                } else if (event.action == GLFW_RELEASE) {
                    // TODO
                }
            });
        } else if (event.key == GLFW_KEY_S) {
            myWorld->each<PathMove>([&](Entity *ent, ComponentHandle<PathMove> pathmove) {
                if (event.action == GLFW_PRESS) {
                    // TODO
                } else if (event.action == GLFW_RELEASE) {
                    // TODO
                }
            });
        }
    }

    void tick(World *pWorld, float deltaTime) override {
        PathMove::Path path;

        path.points = std::vector<glm::vec3>{
            glm::vec3(0.0, 2.0, 0.0),
            glm::vec3(0.0, 2.0, 1.0),
            glm::vec3(2.0, 2.0, 2.0),
            glm::vec3(1.0, 3.0, 3.0),
            glm::vec3(-2.0, 2.0, 4.0),
            glm::vec3(2.0, 2.0, 4.0),
        };

        pWorld->each<Transform, PathMove>(
                [&](Entity *ent, ComponentHandle<Transform> transform, ComponentHandle<PathMove> pathmove) {
                    pathmove->time_passed += deltaTime;

                    // Move from the second to the third point for now
                    glm::vec3 point = catmul(0.5f, path.points[0], path.points[1], path.points[2], path.points[3], pathmove->time_passed * 0.5);
                    transform->set_position(point);
                });
    }

    void unconfigure(World *pWorld) override {
        pWorld->unsubscribeAll(this);
    }

private:
    World *myWorld;
};

#endif // __PATHMOVESYSTEM_H__