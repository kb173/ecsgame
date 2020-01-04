//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_MOVEMENT_H
#define ECSGAME_MOVEMENT_H

struct Movement {
    Movement(float speedX, float speedY, float speedZ) : speedX(speedX), speedY(speedY), speedZ(speedZ) {}

    float speedX;
    float speedY;
    float speedZ;

    int movingX = 0;
    int movingY = 0;
    int movingZ = 0;

    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float velocityZ = 0.0f;
};

#endif //ECSGAME_MOVEMENT_H
