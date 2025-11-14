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

void main()
{
    GUI_ID = i;
    fragPos = instanceData[GUI_ID].pos + quad[gl_VertexID] * instanceData[GUI_ID].size;
    gl_Position = vec4(2.f*fragPos/windowSize - vec2(1, 1), 0, 1);
    gl_Position.y = -gl_Position.y;
    
    fragCol = instanceData[GUI_ID].color;
}
