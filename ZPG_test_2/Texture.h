#pragma once
#include <GL/glew.h>
#include <string>
#include "stb_image.h"

class Texture {
public:
    GLuint id;
    int width, height, channels;

    Texture(const std::string& p);

    void Bind(GLuint slot = 0) const;
};
