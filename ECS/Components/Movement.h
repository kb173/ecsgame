//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_MOVEMENT_H
#define ECSGAME_MOVEMENT_H

struct Movement {
    Movement(glm::vec3 speed) : speed(speed) {}

    glm::vec3 speed;

    glm::ivec3 moving;

    glm::vec3 velocity;
};

#endif //ECSGAME_MOVEMENT_H
