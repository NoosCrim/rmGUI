#pragma once
#include <GL/glew.h>
#include <vector>
namespace rmGUI
{
    class TextureAtlas
    {
        const GLuint maxTextureSize = 2048;
        const uint32_t atlasLayerByteSize = maxTextureSize * maxTextureSize; 
        GLuint textureArray = 0;
        std::vector<uint8_t> layers;
    };
}