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
#include "../Components/ObjMesh.h"
#include "../Components/Texture.h"

using namespace ECS;

class RenderSystem : public EntitySystem {
public:
    void render(World *pWorld, Shader shader) {

        pWorld->each<Camera, Transform>([&](Entity *ent, ComponentHandle<Camera> camera, ComponentHandle<Transform> cameraTransform) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.use();

            shader.setMat4("projection", camera->projection);
            shader.setMat4("view", glm::inverse(cameraTransform->matrix));

            pWorld->each<Mesh, Transform>([&](Entity *ent, ComponentHandle<Mesh> mesh, ComponentHandle<Transform> transform) {
                shader.setMat4("model", transform->matrix);

                mesh->render();
            });

            // TODO: Duplicate of loop above, but for ObjMesh instead of Mesh. Is it possible to do this implicitly via polymorphism?
            pWorld->each<ObjMesh, Transform>([&](Entity *ent, ComponentHandle<ObjMesh> mesh, ComponentHandle<Transform> transform) {
                shader.setMat4("model", transform->matrix);

                mesh->render();
            });

            // Render ObjMeshes with textures
            pWorld->each<ObjMesh, Transform, Texture>([&](Entity *ent, ComponentHandle<ObjMesh> mesh, ComponentHandle<Transform> transform, ComponentHandle<Texture> texture) {
                shader.setMat4("model", transform->matrix);
                glBindTexture(GL_TEXTURE_2D, texture->id);

                mesh->render();
            });
        });
    }

private:
    float gravityAmount;
};

#endif //ECSGAME_RENDERSYSTEM_H
