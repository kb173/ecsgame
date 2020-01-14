//
// Created by karl on 14.01.20.
//

#ifndef ECSGAME_TEXTURE_H
#define ECSGAME_TEXTURE_H

#include "../../Util/stb_image.h"

struct Texture {
    struct Settings {
        Settings(bool mipmaps, bool transparent) : mipmaps(mipmaps), transparent(transparent) {}

        bool mipmaps = true;
        bool transparent = false;
    };

    explicit Texture(const std::string& path, Settings settings) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (settings.mipmaps) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

        if (data) {
            // Alpha channel?
            unsigned int glChannels = GL_RGB;
            if (nrChannels == 4) {
                glChannels = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, glChannels, width, height, 0, glChannels, GL_UNSIGNED_BYTE, data);

            if (settings.mipmaps) {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);

        render_transparent = settings.transparent;
    }

    unsigned int id;
    bool render_transparent = false;
};

#endif //ECSGAME_TEXTURE_H
