#version 460

vec2 quad[6] = {
    vec2(0, 0),
    vec2(1, 0),
    vec2(1, 1),
    vec2(0, 0),
    vec2(1, 1),
    vec2(0, 1)
};

struct InstanceDataStruct
{
    vec4 color;
    vec2 pos, size;
    float height;
};

layout(std430, binding=0) buffer instanceDataBuffer
{
    InstanceDataStruct instanceData[];
};
layout(location=0) uniform ivec2 windowSize;
in uint i;
out vec2 fragPos;
out vec4 fragColor;
flat out float fragHeight;

void main()
{
    fragPos = instanceData[i].pos + quad[gl_VertexID] * instanceData[i].size;
    gl_Position = vec4(2.f*fragPos/windowSize - vec2(1, 1), 0, 1);
    gl_Position.y = -gl_Position.y;
    fragColor = instanceData[i].color;
    fragHeight = instanceData[i].height;
}
