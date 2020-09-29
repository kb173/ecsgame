#ifndef __PATHMOVE_H__
#define __PATHMOVE_H__

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

struct PathMove {
    struct Path {
        Path(std::vector<glm::vec3> points) : points(points) {}

        std::vector<glm::vec3> points;
    };

    struct Views {
        Views(std::vector<glm::quat> views) : views(views) {}

        std::vector<glm::quat> views;
    };

    PathMove(double speed, Path path, Views views) : speed(speed), path(path), views(views) {}

    double speed;
    float time_passed = 0.0;
    int current_point_index = 0;

    Path path;
    Views views;
};
#endif // __PATHMOVE_H__