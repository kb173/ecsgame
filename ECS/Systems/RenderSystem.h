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
            glClearColor(0.6f, 0.9f, 0.9f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.use();

            shader.setMat4("projection", camera->projection);
            shader.setMat4("view", glm::inverse(cameraTransform->matrix));

            std::vector<RenderObject> renderObjects;
            std::vector<RenderObject> transparentRenderObjects;

            pWorld->each<Mesh, Transform>([&](Entity *ent, ComponentHandle<Mesh> mesh, ComponentHandle<Transform> transform) {
                renderObjects.emplace_back(RenderObject(transform->matrix, 0, mesh.get(), 0));
            });

            glm::vec3 cameraPos = cameraTransform->getPosition();

            /*// TODO: Is it possible to do get ObjMeshes in the Mesh loop above implicitly via polymorphism?
             * // TODO: Commented out because of double rendering - we only want to get objects that explicitly DON'T have a texture here!
            pWorld->each<ObjMesh, Transform>([&](Entity *ent, ComponentHandle<ObjMesh> mesh, ComponentHandle<Transform> transform) {
                // Add the object to the renderObjects to draw if the distance is within the min and max distance of the mesh
                float distance = glm::distance(cameraPos, transform->getPosition());

                if (distance > mesh->minDistance && distance < mesh->maxDistance) {
                    renderObjects.emplace_back(RenderObject(transform->matrix, 0, mesh.get(), distance));
                }
            });*/

            // ObjMesh with textures
            pWorld->each<ObjMesh, Transform, Texture>([&](Entity *ent, ComponentHandle<ObjMesh> mesh, ComponentHandle<Transform> transform, ComponentHandle<Texture> texture) {
                // Add the object to the renderObjects to draw if the distance is within the min and max distance of the mesh
                float distance = glm::distance(cameraPos, transform->getPosition());

                if (distance > mesh->minDistance && distance < mesh->maxDistance) {
                    // Put it into the list of transparent render objects if the texture wants to be rendered transparently
                    if (texture->render_transparent) {
                        transparentRenderObjects.emplace_back(RenderObject(transform->matrix, texture->id, mesh.get(), distance));
                    } else {
                        renderObjects.emplace_back(RenderObject(transform->matrix, texture->id, mesh.get(), distance));
                    }
                }
            });

            // LODObjMesh with Texture
            pWorld->each<LODObjMesh, Transform, Texture>([&](Entity *ent, ComponentHandle<LODObjMesh> lodMesh, ComponentHandle<Transform> transform, ComponentHandle<Texture> texture) {
                float distance = glm::distance(cameraPos, transform->getPosition());

                for (const auto &mesh : lodMesh->meshes) {
                    if (distance > mesh.minDistance && distance < mesh.maxDistance) {
                        // Put it into the list of transparent render objects if the texture wants to be rendered transparently
                        if (texture->render_transparent) {
                            transparentRenderObjects.emplace_back(RenderObject(transform->matrix, texture->id, mesh, distance));
                        } else {
                            renderObjects.emplace_back(RenderObject(transform->matrix, texture->id, mesh, distance));
                        }
                    }
                }
            });

            for (const RenderObject &obj : renderObjects) {
                obj.render(shader);
            }

            // Sort transparent objects and render them
            std::sort(transparentRenderObjects.begin(), transparentRenderObjects.end(), [](const RenderObject &first, const RenderObject &second) -> bool {
                return first.distance > second.distance;
            });
            for (const RenderObject &obj : transparentRenderObjects) {
                obj.render(shader);
            }
        });
    }

    struct RenderObject {
        RenderObject(const glm::mat4 &matrix, unsigned int textureId, const Mesh &mesh, float distance)
                                                                                        : matrix(matrix),
                                                                                          texture_id(textureId),
                                                                                          mesh(mesh),
                                                                                          distance(distance) {}

        void render(Shader shader) const {
            shader.setMat4("model", matrix);

            // 0 can't be a valid texture name, so we use it for meshes without textures here
            if (texture_id != 0) {
                glBindTexture(GL_TEXTURE_2D, texture_id);
            }

            mesh.render();
        }

        glm::mat4 matrix;
        unsigned int texture_id;
        Mesh mesh;
        float distance;
    };
};

#endif //ECSGAME_RENDERSYSTEM_H
