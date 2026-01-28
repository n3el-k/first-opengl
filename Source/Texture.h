#pragma once
#include <cstdint>

typedef unsigned int GLenum; 

class Texture
{
public:
    uint32_t ID;

    Texture(const char* texturePath, GLenum colorFormat, bool flipVertically=false);
    ~Texture();

    void bind(uint32_t unit = 0) const;

private:

};