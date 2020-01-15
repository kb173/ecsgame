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
#include "../Components/LODObjMesh.h"

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

            std::vector<RenderObject> renderObjects;

            pWorld->each<Mesh, Transform>([&](Entity *ent, ComponentHandle<Mesh> mesh, ComponentHandle<Transform> transform) {
                renderObjects.emplace_back(RenderObject(transform->matrix, 0, mesh.get()));
            });

            // TODO: Is it possible to do get ObjMeshes in the Mesh loop above implicitly via polymorphism?
            pWorld->each<ObjMesh, Transform>([&](Entity *ent, ComponentHandle<ObjMesh> mesh, ComponentHandle<Transform> transform) {
                // Add the object to the renderObjects to draw if the distance is within the min and max distance of the mesh
                glm::vec3 cameraPos = cameraTransform->getPosition();
                glm::vec3 objPos = transform->getPosition();
                float distance = glm::distance(cameraPos, objPos);

                if (distance > mesh->minDistance && distance < mesh->maxDistance) {
                    renderObjects.emplace_back(RenderObject(transform->matrix, 0, mesh.get()));
                }
            });

            // ObjMesh with textures
            pWorld->each<ObjMesh, Transform, Texture>([&](Entity *ent, ComponentHandle<ObjMesh> mesh, ComponentHandle<Transform> transform, ComponentHandle<Texture> texture) {
                // Add the object to the renderObjects to draw if the distance is within the min and max distance of the mesh
                glm::vec3 cameraPos = cameraTransform->getPosition();
                glm::vec3 objPos = transform->getPosition();
                float distance = glm::distance(cameraPos, objPos);

                if (distance > mesh->minDistance && distance < mesh->maxDistance) {
                    renderObjects.emplace_back(RenderObject(transform->matrix, texture->id, mesh.get()));
                }
            });

            pWorld->each<LODObjMesh, Transform, Texture>([&](Entity *ent, ComponentHandle<LODObjMesh> lodMesh, ComponentHandle<Transform> transform, ComponentHandle<Texture> texture) {
                glm::vec3 cameraPos = cameraTransform->getPosition();
                glm::vec3 objPos = transform->getPosition();
                float distance = glm::distance(cameraPos, objPos);

                for (const auto &mesh : lodMesh->meshes) {
                    if (distance > mesh.minDistance && distance < mesh.maxDistance) {
                        renderObjects.emplace_back(RenderObject(transform->matrix, texture->id, mesh));
                    }
                }
            });

            // TODO: Separate lists for transparent and non-transparent RenderObjects. The non-transparent list is
            //  rendered first, then the transparent list is sorted and rendered.

            for (const RenderObject &obj : renderObjects) {
                shader.setMat4("model", obj.matrix);

                // 0 can't be a valid texture name, so we use it for meshes without textures here
                if (obj.texture_id != 0) {
                    glBindTexture(GL_TEXTURE_2D, obj.texture_id);
                }

                obj.mesh.render();
            }
        });
    }

    struct RenderObject {
        RenderObject(const glm::mat4 &matrix, unsigned int textureId, const Mesh &mesh) : matrix(matrix),
                                                                                          texture_id(textureId),
                                                                                          mesh(mesh) {}

        glm::mat4 matrix;
        unsigned int texture_id;
        Mesh mesh;
    };
};

#endif //ECSGAME_RENDERSYSTEM_H
