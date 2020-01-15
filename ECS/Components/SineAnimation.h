//
// Created by karl on 15.01.20.
//

#ifndef ECSGAME_SINEANIMATION_H
#define ECSGAME_SINEANIMATION_H

struct SineAnimation {
    SineAnimation(const glm::vec3 &maxDistance, float speedScale) : maxDistance(maxDistance), speedScale(speedScale) {}

    glm::vec3 maxDistance;
    float speedScale;
};

#endif //ECSGAME_SINEANIMATION_H
