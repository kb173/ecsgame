//
// Created by karl on 04.01.20.
//

#ifndef ECSGAME_POSITION_H
#define ECSGAME_POSITION_H

struct Position {
    Position(float x, float y, float z) : x(x), y(y), z(z) {}

    Position() : x(0.f), y(0.f), z(0.f) {}

    float x;
    float y;
    float z;
};

#endif //ECSGAME_POSITION_H
