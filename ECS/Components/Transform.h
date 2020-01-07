//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_TRANSFORM_H
#define ECSGAME_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Transform {
    Transform() = default;

    glm::mat4 matrix = glm::mat4(1.0f); // Initialize as identity

    void translate(glm::vec3 offset) {
        matrix = glm::translate(matrix, offset);
    }

    void uniform_scale(float factor) {
        scale(glm::vec3(factor, factor, factor));
    }

    void scale(glm::vec3 factors) {
        matrix = glm::scale(matrix, factors);
    }

    glm::vec3 getPosition() {
        return matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
};

#endif //ECSGAME_TRANSFORM_H
