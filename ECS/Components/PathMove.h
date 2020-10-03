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

    PathMove(double speed, Path path, Views views) : speed(speed), path(path), views(views) {
        // Calculate distances
        for (int i = 0; i < path.points.size() - 1; i++) {
            distances.emplace_back(glm::distance(path.points[i], path.points[i + 1]));
        }
    }

    bool is_active;
    double speed;
    float time_passed = 0.0;
    int current_point_index = 0;

    int speed_addition = 0;  // 0, -1 or 1 depending on whether the speed should stay, decrease or increase

    Path path;
    Views views;
    std::vector<float> distances;
};
#endif // __PATHMOVE_H__