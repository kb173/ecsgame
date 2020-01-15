//
// Created by karl on 15.01.20.
//

#ifndef ECSGAME_LODOBJMESH_H
#define ECSGAME_LODOBJMESH_H

#include <utility>
#include <vector>
#include "ObjMesh.h"

struct LODObjMesh {
    explicit LODObjMesh(std::vector<ObjMesh> meshes) : meshes(std::move(meshes)) {}

    std::vector<ObjMesh> meshes;
};

#endif //ECSGAME_LODOBJMESH_H
