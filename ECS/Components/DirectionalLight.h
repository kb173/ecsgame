//
// Created by karl on 15.01.20.
//

#ifndef ECSGAME_DIRECTIONALLIGHT_H
#define ECSGAME_DIRECTIONALLIGHT_H

struct DirectionalLight {
    explicit DirectionalLight(const glm::vec3 &direction) : direction(direction) {}

    glm::vec3 direction;
};

#endif //ECSGAME_DIRECTIONALLIGHT_H
