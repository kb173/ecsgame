//
// Created by karl on 07.01.20.
//

#ifndef ECSGAME_CAMERA_H
#define ECSGAME_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
    /// Create a camera with a field of view (in degrees), width and height (in any unit) and near and far distances
    Camera(float fov, float width, float height, float near, float far) : projection(
            glm::perspective(glm::radians(fov), width / height, near, far)) {}

    glm::mat4 projection;

    glm::mat4 view;
};

#endif //ECSGAME_CAMERA_H
