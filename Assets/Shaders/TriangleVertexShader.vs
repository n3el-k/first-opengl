#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_texCoords;

out vec3 color;
out vec2 texCoords;

void main()
{
    gl_Position = vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);
    color = in_color;
    texCoords = in_texCoords;
}