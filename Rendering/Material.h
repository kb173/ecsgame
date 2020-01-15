//
// Created by karl on 15.01.20.
//

#ifndef ECSGAME_MATERIAL_H
#define ECSGAME_MATERIAL_H

struct Material {
    Material() = default;

    Material(float diffuse, float specular) : diffuse(diffuse), specular(specular) {}

    float diffuse = 0.8;

    float specular = 0.2;
};

#endif //ECSGAME_MATERIAL_H
