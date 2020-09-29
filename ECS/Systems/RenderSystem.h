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
#include "../Components/DirectionalLight.h"
#include "../../Rendering/Material.h"

using namespace ECS;

class RenderSystem : public EntitySystem {
public:
    void render(World *pWorld, Shader shader) {

        pWorld->each<Camera, Transform>([&](Entity *ent, ComponentHandle<Camera> camera, ComponentHandle<Transform> cameraTransform) {
            glClearColor(0.6f, 0.9f, 0.9f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.use();

            // Lighting
            // TODO: Currently only the last light is used!
            pWorld->each<DirectionalLight>([&](Entity *ent, ComponentHandle<DirectionalLight> light) {
                shader.setVec3("lightDirection", light->direction);
            });

            glm::vec3 cameraPos = cameraTransform->get_origin();

            glm::mat4 view = cameraTransform->matrix;
            view[3] = glm::vec4(cameraPos, 1.0);

            shader.setMat4("projection", camera->projection);
            shader.setMat4("view", glm::inverse(view));
            shader.setVec3("cameraPosition", cameraTransform->get_origin());

            std::vector<RenderObject> renderObjects;
            std::vector<RenderObject> transparentRenderObjects;

            /*pWorld->each<Mesh, Transform>([&](Entity *ent, ComponentHandle<Mesh> mesh, ComponentHandle<Transform> transform) {
                renderObjects.emplace_back(RenderObject(transform->matrix, 0, mesh.get(), 0));
            });*/

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
                float distance = glm::distance(cameraPos, transform->get_origin());

                if (distance > mesh->minDistance && distance < mesh->maxDistance) {
                    // Get optional components
                    ComponentHandle<Texture> textureComponent = ent->get<Texture>();
                    ComponentHandle<Material> materialComponent = ent->get<Material>();

                    Material material = materialComponent.isValid() ? materialComponent.get() : Material();
                    unsigned int textureID = textureComponent.isValid() ? textureComponent->id : 0;

                    // Put it into the list of transparent render objects if the texture wants to be rendered transparently
                    if (textureComponent.isValid() && textureComponent->render_transparent) {
                        transparentRenderObjects.emplace_back(RenderObject(transform->matrix, transform->get_origin(), textureID, mesh.get(), distance, material));
                    } else {
                        renderObjects.emplace_back(RenderObject(transform->matrix, transform->get_origin(), textureID, mesh.get(), distance, material));
                    }
                }
            });

            // LODObjMesh with Texture
            pWorld->each<LODObjMesh, Transform>([&](Entity *ent, ComponentHandle<LODObjMesh> lodMesh, ComponentHandle<Transform> transform) {
                float distance = glm::distance(cameraPos, transform->get_origin());

                for (const auto &mesh : lodMesh->meshes) {
                    if (distance > mesh.minDistance && distance < mesh.maxDistance) {
                        // Get optional components
                        ComponentHandle<Texture> textureComponent = ent->get<Texture>();
                        ComponentHandle<Material> materialComponent = ent->get<Material>();

                        Material material = materialComponent.isValid() ? materialComponent.get() : Material();
                        unsigned int textureID = textureComponent.isValid() ? textureComponent->id : 0;

                        // Put it into the list of transparent render objects if the texture wants to be rendered transparently
                        if (textureComponent.isValid() && textureComponent->render_transparent) {
                            transparentRenderObjects.emplace_back(RenderObject(transform->matrix, transform->get_origin(), textureID, mesh, distance, material));
                        } else {
                            renderObjects.emplace_back(RenderObject(transform->matrix, transform->get_origin(), textureID, mesh, distance, material));
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
        RenderObject(const glm::mat4 &matrix, const glm::vec3 &origin, unsigned int textureId, const Mesh &mesh, float distance, const Material &material)
                                                                                        : matrix(matrix),
                                                                                          origin(origin),
                                                                                          texture_id(textureId),
                                                                                          mesh(mesh),
                                                                                          distance(distance),
                                                                                          material(material) {}

        void render(Shader shader) const {
            glm::mat4 model_matrix = matrix;
            model_matrix[3] = glm::vec4(origin, 1.0);
            shader.setMat4("model", model_matrix);

            // 0 can't be a valid texture name, so we use it for meshes without textures here
            if (texture_id != 0) {
                glBindTexture(GL_TEXTURE_2D, texture_id);
            }

            shader.setFloat("diffuseStrength", material.diffuse);
            shader.setFloat("specularStrength", material.specular);

            mesh.render();
        }

        glm::mat4 matrix;
        glm::vec3 origin;
        unsigned int texture_id;
        Mesh mesh;
        float distance;
        Material material;
    };
};

#endif //ECSGAME_RENDERSYSTEM_H
