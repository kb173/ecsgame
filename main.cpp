#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Rendering/Shader.h"
#include "ECS/ECS.h"

using namespace ECS;


World *world = World::createWorld();


struct Position {
    Position(float x, float y, float z) : x(x), y(y), z(z) {}

    Position() : x(0.f), y(0.f), z(0.f) {}

    float x;
    float y;
    float z;
};

struct Movement {
    Movement(float speedX, float speedY, float speedZ) : speedX(speedX), speedY(speedY), speedZ(speedZ) {}

    float speedX;
    float speedY;
    float speedZ;

    int movingX = 0;
    int movingY = 0;
    int movingZ = 0;

    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float velocityZ = 0.0f;
};

struct InputEvent {
    int key;
    int action;
};

class GravitySystem : public EntitySystem {
public:
    explicit GravitySystem(float amount) {
        gravityAmount = amount;
    }

    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Position>([&](Entity *ent, ComponentHandle<Position> position) {
            position->y += gravityAmount * deltaTime;
        });
    }

private:
    float gravityAmount;
};

class PositionDebugOutputSystem : public EntitySystem {
public:
    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Position>([&](Entity *ent, ComponentHandle<Position> position) {
            std::cout << ent->getEntityId() << ": "
                      << position->x << ", "
                      << position->y << ", "
                      << position->z
                      << std::endl;
        });
    }
};

class KeyboardMovementSystem : public EntitySystem, public EventSubscriber<InputEvent> {
    void configure(World *pWorld) override {
        pWorld->subscribe<InputEvent>(this);
    }

    void receive(World *pWorld, const InputEvent &event) override {
        if (event.key == GLFW_KEY_W) {
            world->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->movingZ = -1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->movingZ = 0;
                }
            });
        } else if (event.key == GLFW_KEY_S) {
            world->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->movingZ = 1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->movingZ = 0;
                }
            });
        } else if (event.key == GLFW_KEY_A) {
            world->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->movingX = 1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->movingX = 0;
                }
            });
        } else if (event.key == GLFW_KEY_D) {
            world->each<Movement>([&](Entity *ent, ComponentHandle<Movement> movement) {
                if (event.action == GLFW_PRESS) {
                    movement->movingX = -1;
                } else if (event.action == GLFW_RELEASE) {
                    movement->movingX = 0;
                }
            });
        }
        std::cout << "MyEvent was emitted!" << std::endl;
    }

    void tick(World *pWorld, float deltaTime) override {
        pWorld->each<Position, Movement>(
                [&](Entity *ent, ComponentHandle<Position> position, ComponentHandle<Movement> movement) {
                    position->x += movement->movingX * movement->speedX * deltaTime;
                    position->y += movement->movingY * movement->speedY * deltaTime;
                    position->z += movement->movingZ * movement->speedZ * deltaTime;
                });
    }

    void unconfigure(World *pWorld) override {
        pWorld->unsubscribeAll(this);
    }
};


static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    world->emit<InputEvent>({key, action});
}

int main() {
    world->registerSystem(new GravitySystem(-9.8f));
    world->registerSystem(new PositionDebugOutputSystem());
    world->registerSystem(new KeyboardMovementSystem());

    Entity *ent = world->create();
    ent->assign<Position>(0.f, 0.f, 0.f);
    ent->assign<Movement>(1.f, 1.f, 1.f);

    ComponentHandle<Position> pos = ent->get<Position>();

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
