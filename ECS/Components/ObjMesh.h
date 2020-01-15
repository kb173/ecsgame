//
// Created by karl on 12.01.20.
//

#ifndef ECSGAME_OBJMESH_H
#define ECSGAME_OBJMESH_H

#include "Mesh.h"
#include "../../Util/OBJ_Loader.h"

struct ObjMesh : public Mesh {
    struct Settings {
        Settings(float minDistanceForRender, float maxDistanceForRender) : minDistanceForRender(
                minDistanceForRender), maxDistanceForRender(maxDistanceForRender) {}

        Settings() = default;

        float minDistanceForRender = 0.0;
        float maxDistanceForRender = 1000.0;
    };

    explicit ObjMesh(const std::string &path, const Settings &settings) : Mesh(getVerticesFromFile(path), getIndicesFromFile(path)),
                                                                                 minDistance(settings.minDistanceForRender),
                                                                                 maxDistance(settings.maxDistanceForRender) {}

    float minDistance;

    float maxDistance;

private:
    static std::vector<float> getVerticesFromFile(const std::string &path) {
        objl::Loader loader;

        bool loadout = loader.LoadFile(path);

        if (loadout) {
            // TODO: Currently only the first mesh is used
            objl::Mesh curMesh = loader.LoadedMeshes[0];

            std::vector<float> vertexData = std::vector<float>();

            // Go through each vertex
            for (auto &Vertice : curMesh.Vertices) {
                vertexData.emplace_back(Vertice.Position.X);
                vertexData.emplace_back(Vertice.Position.Y);
                vertexData.emplace_back(Vertice.Position.Z);
                vertexData.emplace_back(Vertice.Normal.X);
                vertexData.emplace_back(Vertice.Normal.Y);
                vertexData.emplace_back(Vertice.Normal.Z);
                vertexData.emplace_back(Vertice.TextureCoordinate.X);
                vertexData.emplace_back(Vertice.TextureCoordinate.Y);
            }

            return vertexData;
        } else {
            // Output Error
            std::cout << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
        }
    }

    static std::vector<unsigned int> getIndicesFromFile(const std::string &path) {
        objl::Loader loader;

        bool loadout = loader.LoadFile(path);

        if (loadout) {
            // TODO: Currently only the first mesh is used
            objl::Mesh curMesh = loader.LoadedMeshes[0];

            std::vector<unsigned int> indices = std::vector<unsigned int>();

            // Emplace indices into the float vector
            for (int j = 0; j < curMesh.Indices.size(); j++) {
                indices.emplace_back(curMesh.Indices[j]);
            }

            return indices;
        } else {
            // Output Error
            std::cout << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
        }
    }

    // TODO: Only prints at the moment
    static std::vector<float> getMaterialFromFile(const std::string &path) {
        objl::Loader loader;

        bool loadout = loader.LoadFile(path);

        if (loadout) {
            // TODO: Currently only the first mesh is used
            objl::Mesh curMesh = loader.LoadedMeshes[0];

            std::vector<float> vertexData = std::vector<float>();

            // Print Material
            std::cout << "Material: " << curMesh.MeshMaterial.name << "\n";
            std::cout << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", "
                      << curMesh.MeshMaterial.Ka.Z << "\n";
            std::cout << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", "
                      << curMesh.MeshMaterial.Kd.Z << "\n";
            std::cout << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", "
                      << curMesh.MeshMaterial.Ks.Z << "\n";
            std::cout << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
            std::cout << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
            std::cout << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
            std::cout << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
            std::cout << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
            std::cout << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
            std::cout << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
            std::cout << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
            std::cout << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";

            return vertexData;
        } else {
            // Output Error
            std::cout << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
        }
    }
};

#endif //ECSGAME_OBJMESH_H
