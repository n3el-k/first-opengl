#include "Texture.h"
#include <stb/stb_image.h>
#include <cassert>
#include <glad/glad.h>

Texture::Texture(const char *texturePath, GLenum colorFormat, bool flipVertically)
{
    int width, height, numChannels;
    stbi_set_flip_vertically_on_load(flipVertically);

    unsigned char* data = stbi_load(texturePath, &width, &height, &numChannels, 0);
    assert(data && "Faled to load texture");
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

void Texture::bind(uint32_t unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
