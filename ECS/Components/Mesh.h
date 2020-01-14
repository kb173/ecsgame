//
// Created by karl on 07.01.20.
//

#ifndef ECSGAME_MESH_H
#define ECSGAME_MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct Mesh {
    explicit Mesh(const std::vector<float> &_vertices, const std::vector<unsigned int> &_indices) : vertex_count(_indices.size()) {
        // Copy the vertices into a local classic float array. Nothing was displayed without this, maybe
        //  due to weird hidden type incompatibility or out of scope issues?
        float vertices[_vertices.size()];
        std::copy(_vertices.begin(), _vertices.end(), vertices);

        unsigned int indices[_indices.size()];
        std::copy(_indices.begin(), _indices.end(), indices);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void render() const {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int EBO;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int vertex_count;
};

#endif //ECSGAME_MESH_H
