//
// Created by karl on 07.01.20.
//

#ifndef ECSGAME_MOUSELOOK_H
#define ECSGAME_MOUSELOOK_H

struct MouseLook {
    explicit MouseLook(float sensitivity) : sensitivity(sensitivity) {}

    float sensitivity;

    double yaw = 0.0;
    double pitch = 0.0;

    bool is_active;
};

#endif //ECSGAME_MOUSELOOK_H
