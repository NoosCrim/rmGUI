#version 460

in vec2 fragPos;
in vec4 fragColor;
flat in float fragHeight;

out vec4 color;

void main()
{
    color = fragColor;
}