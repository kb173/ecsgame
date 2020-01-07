#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Rendering/Shader.h"
#include "ECS/ECS.h"
#include "ECS/Events/InputEvent.h"
#include "ECS/Systems/GravitySystem.h"
#include "ECS/Systems/PositionDebugSystem.h"
#include "ECS/Systems/KeyboardMovementSystem.h"
#include "ECS/Systems/RenderSystem.h"

using namespace ECS;


World *world = World::createWorld();


static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    world->emit<InputEvent>({key, action});
}

int main() {
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TODO: Could be automated by getting all classes within 'ECS/Systems'
    // world->registerSystem(new GravitySystem(-9.8f));
    world->registerSystem(new PositionDebugOutputSystem());
    world->registerSystem(new KeyboardMovementSystem());
    world->registerSystem(new RenderSystem());

    Entity *player = world->create();
    player->assign<Transform>();
    player->assign<Movement>(glm::vec3(1.f, 1.f, 1.f));
    player->assign<Camera>(70.0f, 640, 480, 0.1f, 100.0f);

    Entity *box = world->create();
    box->assign<Transform>();
    box->get<Transform>()->translate(glm::vec3(0.0f, 0.0f, -10.0f));

    Shader defaultShader("Shaders/default-vertex.vs", "Shaders/default-fragment.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 2  // first Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    double timeInLastFrame = glfwGetTime();
    double elapsed_time = 0.0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // Delta time handling
        double delta = glfwGetTime() - timeInLastFrame;
        timeInLastFrame = glfwGetTime();
        elapsed_time += delta;

        std::cout << "Elapsed time: " << elapsed_time << std::endl;

        world->tick(delta);

        /* Render here */
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        defaultShader.use();
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
