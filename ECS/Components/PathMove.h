#ifndef __PATHMOVE_H__
#define __PATHMOVE_H__

#include <glm/glm.hpp>
#include <vector>

struct PathMove {
    struct Path {
        Path(std::vector<glm::vec3> points) : points(points) {}
        
        std::vector<glm::vec3> points;
    };

    PathMove(double speed, Path path) : speed(speed), path(path) {}

    double speed;

    Path path;

    int current_point_index = 0;

    float time_passed = 0.0;
};
#endif // __PATHMOVE_H__