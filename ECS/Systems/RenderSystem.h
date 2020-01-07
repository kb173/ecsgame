//
// Created by karl on 07.01.20.
//

#ifndef ECSGAME_RENDERSYSTEM_H
#define ECSGAME_RENDERSYSTEM_H

#include "../ECS.h"
#include "../Components/Transform.h"
#include "../Components/Mesh.h"
#include "../Components/Camera.h"
#include "../../Rendering/Shader.h"

using namespace ECS;

class RenderSystem : public EntitySystem {
public:
    void render(World *pWorld, Shader shader) {
        pWorld->each<Camera, Transform>([&](Entity *ent, ComponentHandle<Camera> camera, ComponentHandle<Transform> cameraTransform) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.use();

            shader.setMat4("projection", camera->projection);
            shader.setMat4("view", cameraTransform->matrix);

            pWorld->each<Mesh, Transform>([&](Entity *ent, ComponentHandle<Mesh> mesh, ComponentHandle<Transform> transform) {
                shader.setMat4("model", transform->matrix);

                mesh->render();
            });
        });
    }

private:
    float gravityAmount;
};

#endif //ECSGAME_RENDERSYSTEM_H
