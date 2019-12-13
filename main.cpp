#include <iostream>
#include "ECS.h"

using namespace ECS;

struct Position
{
    Position(float x, float y, float z) : x(x), y(y), z(z) {}
    Position() : x(0.f), y(0.f), z(0.f) {}

    float x;
    float y;
    float z;
};

class GravitySystem : public EntitySystem
{
public:
    GravitySystem(float amount)
    {
        gravityAmount = amount;
    }

    virtual ~GravitySystem() {}

    virtual void tick(World* world, float deltaTime) override
    {
        world->each<Position>([&](Entity* ent, ComponentHandle<Position> position) {
            position->y += gravityAmount * deltaTime;
        });
    }

private:
    float gravityAmount;
};

int main() {
    World* world = World::createWorld();
    world->registerSystem(new GravitySystem(-9.8f));

    Entity* ent = world->create();
    ent->assign<Position>(0.f, 0.f, 0.f); // assign() takes arguments and passes them to the constructor

    world->tick(1.0f);

    ComponentHandle<Position> pos = ent->get<Position>();

    std::cout << "y position after tick: " << pos->y << std::endl;
    return 0;
}
