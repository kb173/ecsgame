//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_TRANSFORM_H
#define ECSGAME_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glm/gtx/string_cast.hpp"

struct Transform {
    Transform() = default;

    glm::mat4 matrix = glm::mat4(1.0f); // Initialize as identity
    glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);

    void translate(glm::vec3 offset) {
        matrix = glm::translate(matrix, offset);
    }

    glm::vec3 get_translation() {
        return glm::vec3(matrix[3]);
    }

    void uniform_scale(float factor) {
        scale(glm::vec3(factor, factor, factor));
    }

    void scale(glm::vec3 factors) {
        matrix = glm::scale(matrix, factors);
    }

    void rotate(float degrees, glm::vec3 axis) {
        matrix = glm::rotate(matrix, glm::radians(degrees), axis);
    }

    void set_origin(glm::vec3 position) {
        origin = position;
    }

    void add_to_origin(glm::vec3 addition) {
        origin += addition;
    }

    void set_rotation_from_quat(glm::quat quaternion) {
        // Remember translation
        glm::vec4 save = matrix[3];

        matrix = glm::mat4_cast(quaternion);
        matrix[3] = save;
    }

    glm::vec3 get_origin() const {
        return origin;
    }

    glm::vec3 forward() const {
        return matrix * glm::vec4(0.0, 0.0, -1.0, 0.0);
    }

    glm::vec3 up() const {
        return  matrix * glm::vec4(0.0, 1.0, 0.0, 0.0);
    }

    glm::vec3 right() const {
        return matrix * glm::vec4(1.0, 0.0, 0.0, 0.0);
    }
};

#endif //ECSGAME_TRANSFORM_H
