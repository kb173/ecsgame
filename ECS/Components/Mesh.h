//
// Created by karl on 07.01.20.
//

#ifndef ECSGAME_MESH_H
#define ECSGAME_MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct Mesh {
    explicit Mesh(const std::vector<float> &_vertices) : vertex_count(_vertices.size()) {
        // Copy the vertices into a local classic float array. Nothing was displayed without this, maybe
        //  due to weird hidden type incompatibility or out of scope issues?
        float vertices[vertex_count];
        std::copy(_vertices.begin(), _vertices.end(), vertices);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
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
};

#endif //ECSGAME_MESH_H
