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

// For Debugging
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

class RenderSystem : public EntitySystem {
public:
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
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_id);
            }

            // TODO: Not always required (not when rendering shadows) - make functions separate?
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

    // Fill the passed vector with render objects of the given world
    std::vector<std::vector<RenderObject>> getRenderObjects(World *world, glm::vec3 cameraPos) {
        std::vector<std::vector<RenderObject>> renderObjects(2);  // First normal, then transparent

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
        world->each<ObjMesh, Transform, Texture>([&](Entity *ent, ComponentHandle<ObjMesh> mesh, ComponentHandle<Transform> transform, ComponentHandle<Texture> texture) {
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
                    renderObjects[1].emplace_back(RenderObject(transform->matrix, transform->get_origin(), textureID, mesh.get(), distance, material));
                } else {
                    renderObjects[0].emplace_back(RenderObject(transform->matrix, transform->get_origin(), textureID, mesh.get(), distance, material));
                }
            }
        });

        // LODObjMesh with Texture
        world->each<LODObjMesh, Transform>([&](Entity *ent, ComponentHandle<LODObjMesh> lodMesh, ComponentHandle<Transform> transform) {
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
                        renderObjects[1].emplace_back(RenderObject(transform->matrix, transform->get_origin(), textureID, mesh, distance, material));
                    } else {
                        renderObjects[0].emplace_back(RenderObject(transform->matrix, transform->get_origin(), textureID, mesh, distance, material));
                    }
                }
            }
        });

        // Sort transparent objects
        std::sort(renderObjects[1].begin(), renderObjects[1].end(), [](const RenderObject &first, const RenderObject &second) -> bool {
            return first.distance > second.distance;
        });

        return renderObjects;
    }

    RenderSystem() {
        // Configure depth map
        glGenFramebuffers(1, &depthMapFBO);

        // Create depth texture
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        // Attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    void render(World *pWorld, Shader normalShader, Shader shadowShader, Shader debugShader) {
        pWorld->each<Camera, Transform>([&](Entity *ent, ComponentHandle<Camera> camera, ComponentHandle<Transform> cameraTransform) {
            // Get render objects
            std::vector<std::vector<RenderObject>> allRenderObjects = getRenderObjects(pWorld, cameraTransform->get_origin());
            std::vector<RenderObject> renderObjects = allRenderObjects[0];
            std::vector<RenderObject> transparentRenderObjects = allRenderObjects[1];

            // Calculate matrix for lighting
            // Get light direction
            // TODO: Currently only the last light is used!
            glm::vec3 lightDirection;
            pWorld->each<DirectionalLight>([&](Entity *ent, ComponentHandle<DirectionalLight> light) {
                lightDirection = light->direction;
            });
            float near_plane = 1.0f, far_plane = 100.0f;
            glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
            glm::mat4 lightView = glm::lookAt(lightDirection * 40.0f, -lightDirection, glm::vec3(0.0, 1.0, 0.0));
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;

            // Render shadows
            shadowShader.use();

            shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
            
            glViewport(0, 0, shadow_width, shadow_height);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            for (const RenderObject &obj : renderObjects) {
                obj.render(shadowShader);
            }
            // Don't render transparent objects -- we just assume they don't cast any shadow
            /* for (const RenderObject &obj : transparentRenderObjects) {
                obj.render(shadowShader);
            } */
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Render Normal
            glViewport(0, 0, screen_width, screen_height);
            
            glClearColor(0.6f, 0.9f, 0.9f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            normalShader.use();

            // Lighting
            normalShader.setVec3("lightDirection", lightDirection);
            normalShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

            glm::vec3 cameraPos = cameraTransform->get_origin();

            glm::mat4 view = cameraTransform->matrix;
            view[3] = glm::vec4(cameraPos, 1.0);

            normalShader.setMat4("projection", camera->projection);
            normalShader.setMat4("view", glm::inverse(view));
            normalShader.setVec3("cameraPosition", cameraTransform->get_origin());

            // Bind shadow texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMap);

            for (const RenderObject &obj : renderObjects) {
                obj.render(normalShader);
            }
            for (const RenderObject &obj : transparentRenderObjects) {
                obj.render(normalShader);
            }

            // Render the light's depth map to a quad for debugging
            debugShader.use();
            debugShader.setFloat("near_plane", near_plane);
            debugShader.setFloat("far_plane", far_plane);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            // renderQuad();
        });
    }

    int screen_width = 1280;
    int screen_height = 720;

    int shadow_width = 2048;
    int shadow_height = 2048;

    unsigned int depthMap;
    unsigned int depthMapFBO;
};

#endif //ECSGAME_RENDERSYSTEM_H
