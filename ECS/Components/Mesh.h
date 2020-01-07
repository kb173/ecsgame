//
// Created by karl on 07.01.20.
//

#ifndef ECSGAME_MESH_H
#define ECSGAME_MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct Mesh {
    explicit Mesh(std::vector<float> vertices_vector) : vertices(&vertices_vector[0]), vertex_count(vertices_vector.size()) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void render() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    }

private:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int vertex_count;

    /// Vertices in the format x, y, z, u, v
    float *vertices;
};

#endif //ECSGAME_MESH_H
