//
// Created by karl on 14.01.20.
//

#ifndef ECSGAME_TEXTURE_H
#define ECSGAME_TEXTURE_H

#include "../../Util/stb_image.h"

struct Texture {
    explicit Texture(const std::string& path) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Mipmapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D); // Mipmapping
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);
    }

    unsigned int id;
};

#endif //ECSGAME_TEXTURE_H
