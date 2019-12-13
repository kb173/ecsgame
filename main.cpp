#include <iostream>
#include "ECS.h"
#include <GLFW/glfw3.h>

using namespace ECS;

struct Position {
    Position(float x, float y, float z) : x(x), y(y), z(z) {}

    Position() : x(0.f), y(0.f), z(0.f) {}

    float x;
    float y;
    float z;
};

class GravitySystem : public EntitySystem {
public:
    GravitySystem(float amount) {
        gravityAmount = amount;
    }

    virtual ~GravitySystem() {}

    virtual void tick(World *world, float deltaTime) override {
        world->each<Position>([&](Entity *ent, ComponentHandle<Position> position) {
            position->y += gravityAmount * deltaTime;
        });
    }

private:
    float gravityAmount;
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
    World *world = World::createWorld();
    world->registerSystem(new GravitySystem(-9.8f));

    Entity *ent = world->create();
    ent->assign<Position>(0.f, 0.f, 0.f); // assign() takes arguments and passes them to the constructor

    world->tick(1.0f);

    ComponentHandle<Position> pos = ent->get<Position>();

    std::cout << "y position after tick: " << pos->y << std::endl;

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
