#ifndef __PATHMOVE_H__
#define __PATHMOVE_H__

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

struct PathMove {
    struct Path {
        Path(std::vector<glm::vec3> points) : points(points) {
            // Calculate distances
            for (int i = 0; i < points.size() - 1; i++) {
                distances.emplace_back(glm::distance(points[i], points[i + 1]));
            }
        }

        void add_point(glm::vec3 new_point) {
            distances.emplace_back(glm::distance(points.back(), new_point));
            points.emplace_back(new_point);
        }

        std::vector<glm::vec3> points;
        std::vector<float> distances;
    };

    struct Views {
        Views(std::vector<glm::quat> views) : views(views) {}

        void add_view(glm::quat new_quat) {
            views.emplace_back(new_quat);
        }

        std::vector<glm::quat> views;
    };

    PathMove(double speed, Path path, Views views) : speed(speed), path(path), views(views) {}

    bool is_active = false;
    double speed;
    float time_passed = 0.0;
    int current_point_index = 0;

    int speed_addition = 0;  // 0, -1 or 1 depending on whether the speed should stay, decrease or increase

    Path path;
    Views views;
};
#endif // __PATHMOVE_H__