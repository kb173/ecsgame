#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Rendering/Shader.h"
#include "ECS/ECS.h"
#include "ECS/Events/InputEvent.h"
#include "ECS/Events/MouseMoveEvent.h"
#include "ECS/Systems/GravitySystem.h"
#include "ECS/Systems/PositionDebugSystem.h"
#include "ECS/Systems/KeyboardMovementSystem.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/MouseLookSystem.h"
#include "ECS/Systems/PathMoveSystem.h"
#include "ECS/Components/ObjMesh.h"
#include "ECS/Components/Texture.h"
#include "ECS/Components/SineAnimation.h"
#include "ECS/Systems/SineAnimationSystem.h"
#include "ECS/Components/DirectionalLight.h"
#include "ECS/Components/PathMove.h"
#include "ECS/Systems/InteractivePathSystem.h"

using namespace ECS;


World *world = World::createWorld();


static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    world->emit<InputEvent>({key, action});
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    world->emit<MouseMoveEvent>({xpos, ypos});
}

int main() {
    // TODO: Move this to RenderSystem?
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "ECSGame", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // TODO: Could be automated by getting all classes within 'ECS/Systems'
    // world->registerSystem(new GravitySystem(-9.8f));
    // world->registerSystem(new PositionDebugOutputSystem());
    world->registerSystem(new KeyboardMovementSystem());
    world->registerSystem(new MouseLookSystem(1280, 720));
    world->registerSystem(new PathMoveSystem());
    world->registerSystem(new SineAnimationSystem());
    world->registerSystem(new InteractivePathSystem());

    RenderSystem* renderSystem = new RenderSystem();
    world->registerSystem(renderSystem);

    Entity *player = world->create();
    player->assign<Transform>();
    player->assign<Movement>(glm::vec3(2.f, 2.f, 2.f));
    player->assign<MouseLook>(0.1);
    player->assign<Camera>(70.0f, 1280, 720, 0.1f, 100.0f);
    player->assign<PathMove>(3.0, PathMove::Path(std::vector<glm::vec3>{
            glm::vec3(0.0, 2.0, 0.0),
            glm::vec3(0.0, 2.0, -1.0),
            glm::vec3(0.0, 2.0, -2.0),
            glm::vec3(4.0, 3.0, -3.0),
            glm::vec3(2.0, 2.0, -4.0),
            glm::vec3(1.0, 2.0, -4.0),
            glm::vec3(0.0, 2.0, -10.0)
        }),
        PathMove::Views(std::vector<glm::quat>{
            glm::angleAxis(glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f)),
            glm::angleAxis(glm::radians(10.f), glm::vec3(0.f, 1.f, 0.f)),
            glm::angleAxis(glm::radians(20.f), glm::vec3(0.f, 1.f, 0.f)),
            glm::angleAxis(glm::radians(40.f), glm::vec3(0.f, 1.f, 0.f)),
            glm::angleAxis(glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f)),
            glm::angleAxis(glm::radians(120.f), glm::vec3(0.f, 1.f, 0.f)),
            glm::angleAxis(glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f))
        })
    );

    Entity *monkey = world->create();
    monkey->assign<Transform>();
    monkey->assign<LODObjMesh>(std::vector{ObjMesh("Resources/Monkey.obj",ObjMesh::Settings(0.0, 8.0, 0.4, 0.6)),
                                           ObjMesh("Resources/MonkeySimple.obj", ObjMesh::Settings(8.0, 100.0, 0.4, 0.6))});
    monkey->assign<Texture>("Resources/Marble.jpg", Texture::Settings(true, false));
    monkey->assign<SineAnimation>(glm::vec3(0.0, 0.3, 0.0), 0.5);
    monkey->assign<Material>(0.6, 0.6);
    monkey->get<Transform>()->set_origin(glm::vec3(0.0f, 2.0f, -6.0f));

    Entity *wall1 = world->create();
    wall1->assign<Transform>();
    wall1->assign<ObjMesh>(ObjMesh("Resources/Wall.obj", ObjMesh::Settings()));
    wall1->assign<Texture>("Resources/Glass.png", Texture::Settings(true, true));
    wall1->assign<Material>(0.2, 0.8);
    wall1->get<Transform>()->set_origin(glm::vec3(0.0f, 0.0f, -2.0f));

    Entity *wall2 = world->create();
    wall2->assign<Transform>();
    wall2->assign<ObjMesh>(ObjMesh("Resources/Wall.obj", ObjMesh::Settings()));
    wall2->assign<Texture>("Resources/Glass.png", Texture::Settings(true, true));
    wall2->assign<Material>(0.2, 0.8);
    wall2->get<Transform>()->set_origin(glm::vec3(0.0f, 0.0f, -10.0f));

    Entity *wall3 = world->create();
    wall3->assign<Transform>();
    wall3->assign<ObjMesh>(ObjMesh("Resources/Wall.obj", ObjMesh::Settings()));
    wall3->assign<Texture>("Resources/Glass.png", Texture::Settings(true, true));
    wall3->assign<Material>(0.2, 0.8);
    wall3->get<Transform>()->set_origin(glm::vec3(4.0f, 0.0f, -6.0f));
    wall3->get<Transform>()->rotate(90.0, glm::vec3(0.0, 1.0, 0.0));

    Entity *wall4 = world->create();
    wall4->assign<Transform>();
    wall4->assign<ObjMesh>(ObjMesh("Resources/Wall.obj", ObjMesh::Settings()));
    wall4->assign<Texture>("Resources/Glass.png", Texture::Settings(true, true));
    wall4->assign<Material>(0.2, 0.8);
    wall4->get<Transform>()->set_origin(glm::vec3(-4.0f, 0.0f, -6.0f));
    wall4->get<Transform>()->rotate(90.0, glm::vec3(0.0, 1.0, 0.0));

    Entity *ground = world->create();
    ground->assign<Transform>();
    ground->assign<ObjMesh>(ObjMesh("Resources/Ground.obj", ObjMesh::Settings()));
    ground->assign<Texture>("Resources/Grass.jpg", Texture::Settings(true, false));
    ground->assign<Material>(1.0, 0.0);
    ground->get<Transform>()->set_origin(glm::vec3(0.0f, 0.0f, 0.0f));

    Entity *sun = world->create();
    sun->assign<DirectionalLight>(glm::normalize(glm::vec3(1.0, 1.0, 1.0)));

    Shader defaultShader("Shaders/default-vertex.vs", "Shaders/default-fragment.fs");

    double timeInLastFrame = glfwGetTime();
    double elapsed_time = 0.0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // Delta time handling
        double delta = glfwGetTime() - timeInLastFrame;
        timeInLastFrame = glfwGetTime();
        elapsed_time += delta;

        world->tick(delta);
        renderSystem->render(world, defaultShader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
