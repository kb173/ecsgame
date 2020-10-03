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
                    pathmove->speed_addition += 1;
                } else if (event.action == GLFW_RELEASE) {
                    pathmove->speed_addition -= 1;
                }
            });
        } else if (event.key == GLFW_KEY_S) {
            myWorld->each<PathMove>([&](Entity *ent, ComponentHandle<PathMove> pathmove) {
                if (event.action == GLFW_PRESS) {
                    pathmove->speed_addition -= 1;
                } else if (event.action == GLFW_RELEASE) {
                    pathmove->speed_addition += 1;
                }
            });
        }
    }

    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Transform, PathMove>(
                [&](Entity *ent, ComponentHandle<Transform> transform, ComponentHandle<PathMove> pathmove) {
                    if (!pathmove->is_active) return;

                    // Handle change in speed
                    pathmove->speed += pathmove->speed_addition * deltaTime;
                    pathmove->speed = glm::clamp(pathmove->speed, 0.0, 10.0);

                    // Shorthand for the path (we'll use this a lot)
                    PathMove::Path path = pathmove->path;

                    // Add the passed time
                    float desired_distance = deltaTime * pathmove->speed;  // TODO
                    pathmove->time_passed += desired_distance / path.distances[pathmove->current_point_index];

                    // Shorthand for number of points in the path
                    int num_points = path.points.size();

                    if (pathmove->time_passed >= 1.0) {
                        // If we passed the last target, set the current_point_index to that target
                        pathmove->time_passed -= 1.0;
                        pathmove->current_point_index += 1;

                        // If the point index is greater than the second to last one, reset
                        // (The point index specifies the point we're coming from, not the one we're moving towards)
                        if (pathmove->current_point_index >= num_points - 1) {
                            pathmove->current_point_index = 0;
                        }
                    }

                    // The four points which are needed for the spline
                    // p1 and p2 are always the same (the current origin and the current target), but the rest depends on edge cases
                    glm::vec3 p0;
                    glm::vec3 p1 = path.points[pathmove->current_point_index];
                    glm::vec3 p2 = path.points[pathmove->current_point_index + 1];
                    glm::vec3 p3;

                    if (pathmove->current_point_index == num_points - 2) {
                        // We're moving towards the last point, so the point after that needs to be interpolated.
                        // We interpolate linearly along the line from this point to the target point.
                        glm::vec3 interp_direction = p2 - p1;
                        p3 = p2 + interp_direction * 2.0f;
                    } else {
                        // We're fine - use the point after the target for p3
                        p3 = path.points[pathmove->current_point_index + 2];
                    }

                    if (pathmove->current_point_index == 0) {
                        // We're at the first point, so the point before this needs to be interpolated.
                        // We interpolate linearly along the line from this to the next point (backwards).
                        glm::vec3 interp_direction = path.points[pathmove->current_point_index] - path.points[pathmove->current_point_index + 1];
                        p0 = path.points[pathmove->current_point_index] + interp_direction;
                    } else {
                        // We're fine - use the point before the current point
                        p0 = path.points[pathmove->current_point_index - 1];
                    }

                    // Calculate the point on the spline
                    glm::vec3 point = catmul(1.f,
                            p0,
                            p1,
                            p2,
                            p3,
                            pathmove->time_passed);
                    
                    // Apply
                    transform->set_origin(point);

                    // Rotation
                    // https://www.3dgep.com/understanding-quaternions/#SQUAD
                    PathMove::Views views = pathmove->views;

                    // Similar procedure as with position to get the relevant values
                    glm::quat q0;
                    glm::quat q1 = views.views[pathmove->current_point_index];
                    glm::quat q2 = views.views[pathmove->current_point_index + 1];
                    glm::quat q3;

                    if (pathmove->current_point_index == num_points - 2) {
                        // Interpolate what q3 would be if the change from q1 to q2 continues
                        q3 = glm::fastMix(q1, q2, 2.0f);
                    } else {
                        // We're fine - use the point after the target for p3
                        q3 = views.views[pathmove->current_point_index + 2];
                    }

                    if (pathmove->current_point_index == 0) {
                        // Interpolate what q0 would be if the same change happened from q0 to q1 as from q1 to q2
                        q0 = glm::fastMix(q1, q2, -1.0f);
                    } else {
                        // We're fine - use the point before the current point
                        q0 = views.views[pathmove->current_point_index - 1];
                    }

                    // Interpolate
                    glm::quat result = glm::squad(q1, q2, glm::intermediate(q0, q1, q2), glm::intermediate(q1, q2, q3), pathmove->time_passed);

                    // Apply
                    transform->set_rotation_from_quat(result);

                    std::cout << pathmove->time_passed << std::endl;
                });
    }

    void unconfigure(World *pWorld) override {
        pWorld->unsubscribeAll(this);
    }

private:
    World *myWorld;
};

#endif // __PATHMOVESYSTEM_H__