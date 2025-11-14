#version 460

#include "guiElem.defs.glsl"



vec2 quad[4] = {
    vec2(0, 0),
    vec2(1, 0),
    vec2(1, 1),
    vec2(0, 1)
};

in uint i;
out vec2 fragPos;
out vec4 fragCol;
flat out uint GUI_ID;
flat out vec2 cornerSize[4];

void main()
{
    GUI_ID = i;
    fragPos = instanceData[GUI_ID].pos + quad[gl_VertexID] * instanceData[GUI_ID].size;
    gl_Position = vec4(2.f*fragPos/windowSize - vec2(1, 1), 0, 1);
    gl_Position.y = -gl_Position.y;
    
    fragCol = instanceData[GUI_ID].color;
    cornerSize[0] = instanceData[GUI_ID].tlCornerAbsSize + instanceData[GUI_ID].tlCornerRelSize * instanceData[GUI_ID].size;
    cornerSize[1] = instanceData[GUI_ID].trCornerAbsSize + instanceData[GUI_ID].trCornerRelSize * instanceData[GUI_ID].size;
    cornerSize[2] = instanceData[GUI_ID].brCornerAbsSize + instanceData[GUI_ID].brCornerRelSize * instanceData[GUI_ID].size;
    cornerSize[3] = instanceData[GUI_ID].blCornerAbsSize + instanceData[GUI_ID].blCornerRelSize * instanceData[GUI_ID].size;
}
